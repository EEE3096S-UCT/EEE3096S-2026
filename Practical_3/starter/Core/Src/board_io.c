/* Supplied board support for the UCT STM32F051 family board.
 * LCD: D4 PB8, D5 PB9, D6 PA12, D7 PA15, RS PC14, E PC15.
 * Hold SW3 during reset to swap RS/E on a board with reversed control lines.
 * PA13/PA14 remain available for SWD.
 * All LCD delays and button handling run in the foreground.
 */
#include "board_io.h"
#include "main.h"
#include <stddef.h>
#include <string.h>

#define BUTTON_MASK 0x0fu
#define DEBOUNCE_MS 20u
#define STATE_WAIT_POLLS 100000u

static uint32_t lcd_rs_pin;
static uint32_t lcd_e_pin;
static char lcd_previous[2][16];
static bool lcd_previous_valid;
static bool buttons_initialized;
static uint8_t button_candidate;
static uint8_t button_stable;
static uint32_t button_changed_ms[4];
static bool converters_prepared;

static bool wait_register(volatile uint32_t *reg, uint32_t mask, bool set)
{
    for (uint32_t remaining = STATE_WAIT_POLLS; remaining != 0u; --remaining)
    {
        if (((*reg & mask) != 0u) == set) return true;
    }
    return false;
}

bool board_converters_prepare(void)
{
    converters_prepared = false;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    (void)RCC->AHBENR;
    (void)RCC->APB2ENR;
    (void)RCC->APB1ENR;

    /* PA4 = DAC_OUT1; PA5 = ADC_IN5; no pulls in analog mode. */
    const uint32_t analog_pins = (3u << 8) | (3u << 10);
    GPIOA->MODER |= analog_pins;
    GPIOA->PUPDR &= ~analog_pins;
    RCC->APB2RSTR |= RCC_APB2RSTR_ADCRST;
    RCC->APB2RSTR &= ~RCC_APB2RSTR_ADCRST;
    RCC->APB1RSTR |= RCC_APB1RSTR_DACRST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_DACRST;

    /* Preserve factory trim; run the dedicated 14 MHz ADC oscillator. */
    RCC->CR2 = (RCC->CR2 & ~RCC_CR2_HSI14DIS) | RCC_CR2_HSI14ON;
    if (!wait_register(&RCC->CR2, RCC_CR2_HSI14RDY, true)) return false;
    ADC1->CFGR2 &= ~ADC_CFGR2_CKMODE;
    ADC1->CR |= ADC_CR_ADCAL;
    if (!wait_register(&ADC1->CR, ADC_CR_ADCAL, false)) return false;

    /* ES0202: wait at least four ADC clocks after calibration. At 48 MHz,
     * 32 NOPs alone exceed that interval; loop overhead adds margin. */
    for (uint32_t settle = 0u; settle < 32u; ++settle) __NOP();
    converters_prepared = true;
    return true;
}

bool board_adc_enable(void)
{
    if (!converters_prepared) return false;
    ADC1->ISR = ADC_ISR_ADRDY; /* Write one to clear, no read-modify-write. */
    for (uint32_t remaining = STATE_WAIT_POLLS; remaining != 0u; --remaining)
    {
        /* ES0202: retry enable if hardware cleared ADEN. */
        if ((ADC1->CR & ADC_CR_ADEN) == 0u) ADC1->CR |= ADC_CR_ADEN;
        if ((ADC1->ISR & ADC_ISR_ADRDY) != 0u) return true;
    }
    return false;
}

static void gpio_output(GPIO_TypeDef *port, uint32_t pin)
{
    uint32_t shift = pin * 2u;
    port->MODER = (port->MODER & ~(3u << shift)) | (1u << shift);
    port->OTYPER &= ~(1u << pin);
    port->OSPEEDR = (port->OSPEEDR & ~(3u << shift)) | (3u << shift);
    port->PUPDR &= ~(3u << shift);
}

static void lcd_delay_us(uint32_t us)
{
    /* At 48 MHz each taken loop takes four cycles (SUBS 1, BNE 3).
     * Interrupts may lengthen an LCD pulse, which is harmless. */
    uint32_t loops = us * 12u;
    if (loops == 0u) return;
    __asm volatile (
        ".syntax unified\n"
        "1: subs %0, %0, #1\n"
        "bne 1b\n"
        : "+l" (loops) : : "cc");
}

static void lcd_write_nibble(uint8_t nibble)
{
    uint32_t a_set = 0u;
    uint32_t b_set = 0u;
    if ((nibble & 1u) != 0u) b_set |= 1u << 8;
    if ((nibble & 2u) != 0u) b_set |= 1u << 9;
    if ((nibble & 4u) != 0u) a_set |= 1u << 12;
    if ((nibble & 8u) != 0u) a_set |= 1u << 15;
    GPIOA->BSRR = a_set | ((((1u << 12) | (1u << 15)) & ~a_set) << 16);
    GPIOB->BSRR = b_set | ((((1u << 8) | (1u << 9)) & ~b_set) << 16);
    lcd_delay_us(2u);
    GPIOC->BSRR = lcd_e_pin;
    lcd_delay_us(10u); /* Allow the NMOS level-shifter pull-up to settle. */
    GPIOC->BSRR = lcd_e_pin << 16;
    lcd_delay_us(2u);
}

static void lcd_write(uint8_t byte, bool data)
{
    GPIOC->BSRR = data ? lcd_rs_pin : (lcd_rs_pin << 16);
    lcd_write_nibble((uint8_t)(byte >> 4));
    lcd_write_nibble((uint8_t)(byte & 0x0fu));
    lcd_delay_us(45u);
}

static void lcd_initialize(void)
{
    GPIOC->BSRR = (lcd_rs_pin | lcd_e_pin) << 16;
    HAL_Delay(50u);
    lcd_write_nibble(3u);
    HAL_Delay(5u);
    lcd_write_nibble(3u);
    lcd_delay_us(150u);
    lcd_write_nibble(3u);
    lcd_delay_us(150u);
    lcd_write_nibble(2u);
    lcd_delay_us(150u);
    lcd_write(0x28u, false); /* Four-bit interface, two lines. */
    lcd_write(0x08u, false);
    lcd_write(0x01u, false);
    HAL_Delay(3u);
    lcd_write(0x06u, false);
    lcd_write(0x0cu, false);
    lcd_previous_valid = false;
}

void board_io_init(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;
    (void)RCC->AHBENR;
    GPIOA->MODER &= ~0xffu;
    GPIOA->PUPDR = (GPIOA->PUPDR & ~0xffu) | 0x55u;
    bool swap_controls = (GPIOA->IDR & (1u << 3)) == 0u;
    lcd_rs_pin = 1u << (swap_controls ? 15u : 14u);
    lcd_e_pin = 1u << (swap_controls ? 14u : 15u);

    GPIOA->BSRR = ((1u << 12) | (1u << 15)) << 16;
    GPIOB->BSRR = (1u << 12) | (((1u << 8) | (1u << 9)
                              | (1u << 10) | (1u << 11)) << 16);
    GPIOC->BSRR = ((1u << 14) | (1u << 15)) << 16;
    gpio_output(GPIOA, 12u);
    gpio_output(GPIOA, 15u);
    gpio_output(GPIOB, 8u);
    gpio_output(GPIOB, 9u);
    gpio_output(GPIOB, 10u); /* Red LED. */
    gpio_output(GPIOB, 11u); /* Green LED. */
    gpio_output(GPIOB, 12u); /* EEPROM chip select: inactive high. */
    gpio_output(GPIOC, 14u);
    gpio_output(GPIOC, 15u);
    buttons_initialized = false;
    lcd_initialize();
    board_status(false);
}

uint8_t board_buttons_poll(uint32_t now_ms)
{
    uint8_t raw = (uint8_t)(~GPIOA->IDR) & BUTTON_MASK;
    uint8_t pressed = 0u;
    if (!buttons_initialized)
    {
        button_candidate = raw;
        button_stable = raw;
        for (uint32_t i = 0u; i != 4u; ++i) button_changed_ms[i] = now_ms;
        buttons_initialized = true;
        return 0u;
    }
    for (uint32_t i = 0u; i != 4u; ++i)
    {
        uint8_t bit = (uint8_t)(1u << i);
        if ((raw & bit) != (button_candidate & bit))
        {
            button_candidate = (uint8_t)((button_candidate & (uint8_t)~bit) | (raw & bit));
            button_changed_ms[i] = now_ms;
        }
        if ((uint32_t)(now_ms - button_changed_ms[i]) >= DEBOUNCE_MS
            && (button_stable & bit) != (button_candidate & bit))
        {
            button_stable = (uint8_t)((button_stable & (uint8_t)~bit) | (button_candidate & bit));
            if ((button_stable & bit) != 0u) pressed |= bit;
        }
    }
    return pressed;
}

void board_lcd_show(const char *line1, const char *line2)
{
    const char *lines[2] = {line1, line2};
    for (uint32_t row = 0u; row != 2u; ++row)
    {
        char padded[16];
        bool ended = (lines[row] == NULL);
        for (uint32_t col = 0u; col != 16u; ++col)
        {
            if (!ended && lines[row][col] == '\0') ended = true;
            padded[col] = ended ? ' ' : lines[row][col];
        }
        if (!lcd_previous_valid || memcmp(padded, lcd_previous[row], sizeof padded) != 0)
        {
            lcd_write(row == 0u ? 0x80u : 0xc0u, false);
            for (uint32_t col = 0u; col != 16u; ++col) lcd_write((uint8_t)padded[col], true);
            memcpy(lcd_previous[row], padded, sizeof padded);
        }
    }
    lcd_previous_valid = true;
}

void board_status(bool fault)
{
    GPIOB->BSRR = fault ? ((1u << 10) | (1u << (11u + 16u)))
                       : ((1u << 11) | (1u << (10u + 16u)));
}
