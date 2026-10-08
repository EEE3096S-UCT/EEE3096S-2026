/* EEE3096S Practical 3: Voltage Mirror.
 * Supplied infrastructure: clock, LCD/buttons and fixed sampling timing.
 * Complete only voltage_mirror.c for the assessed converter task.
 */
#include "main.h"
#include "board_io.h"
#include "sampling.h"
#include "voltage_mirror.h"

/* Format one four-digit code without pulling printf into the firmware. */
static void code_digits(char *out, uint16_t code)
{
    out[0] = (char)('0' + (code / 1000u) % 10u);
    out[1] = (char)('0' + (code / 100u) % 10u);
    out[2] = (char)('0' + (code / 10u) % 10u);
    out[3] = (char)('0' + code % 10u);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    board_io_init();
    board_lcd_show("Voltage Mirror", "Starting...");

    bool initialized = voltage_mirror_init();
    bool started = initialized && sampling_start();
    bool coarse = false;
    uint32_t last_display_ms = HAL_GetTick() - 200u;
    if (!initialized)
    {
        board_status(true);
        board_lcd_show("Check TODOs", "Converter init");
    }
    else if (!started)
    {
        board_status(true);
        board_lcd_show("Sampling stopped", "Check clock");
    }

    while (1)
    {
        uint32_t now_ms = HAL_GetTick();
        uint8_t presses = board_buttons_poll(now_ms);
        if (started && (presses & 1u) != 0u)
        {
            coarse = !coarse;
            sampling_set_coarse(coarse);
        }
        if (started && (uint32_t)(now_ms - last_display_ms) >= 200u)
        {
            sampling_snapshot_t state;
            sampling_snapshot(&state);
            last_display_ms = now_ms;
            if (state.fault)
            {
                board_status(true);
                board_lcd_show("Sampling stopped", "ADC read failed");
                started = false;
            }
            else if (state.valid)
            {
                char codes[] = "C0000 D0000";
                code_digits(&codes[1], state.adc_code);
                code_digits(&codes[7], state.dac_code);
                board_lcd_show(state.coarse ? "COARSE6" : "FULL12", codes);
            }
        }
    }
}

/* Supplied: 8 MHz crystal -> PLL x6 -> 48 MHz, undivided AHB/APB.
 * HAL_RCC_ClockConfig also updates the SysTick millisecond time base. */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef oscillator = {0};
    RCC_ClkInitTypeDef clocks = {0};
    oscillator.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    oscillator.HSEState = RCC_HSE_ON;
    oscillator.PLL.PLLState = RCC_PLL_ON;
    oscillator.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    oscillator.PLL.PREDIV = RCC_PREDIV_DIV1;
    oscillator.PLL.PLLMUL = RCC_PLL_MUL6;
    if (HAL_RCC_OscConfig(&oscillator) != HAL_OK) Error_Handler();

    clocks.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
                     | RCC_CLOCKTYPE_PCLK1;
    clocks.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clocks.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clocks.APB1CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&clocks, FLASH_LATENCY_1) != HAL_OK) Error_Handler();
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    (void)file;
    (void)line;
    Error_Handler();
}
#endif
