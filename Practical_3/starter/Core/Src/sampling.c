/* Supplied fixed-rate acquisition. Students implement voltage_mirror.c.
 * TIM2 runs at 48 MHz/(48 * 500) = 2000 samples/s. The interrupt performs
 * one bounded ADC read and one DAC write; LCD/buttons stay in main().
 */
#include "sampling.h"
#include "voltage_mirror.h"
#include "main.h"
#include <stddef.h>

static volatile uint16_t latest_adc;
static volatile uint16_t latest_dac;
static volatile bool requested_coarse;
static volatile bool latest_coarse;
static volatile bool sample_valid;
static volatile bool sampling_fault;
/* Development-only SWD evidence of successful, real acquisitions. */
static volatile uint32_t sample_count;

bool sampling_start(void)
{
    /* The supplied clock setup uses an undivided 48 MHz APB. */
    if (SystemCoreClock != 48000000u || HAL_RCC_GetPCLK1Freq() != 48000000u)
    {
        sampling_fault = true;
        return false;
    }
    NVIC_DisableIRQ(TIM2_IRQn);
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    (void)RCC->APB1ENR;
    RCC->APB1RSTR |= RCC_APB1RSTR_TIM2RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM2RST;
    TIM2->PSC = 47u;
    TIM2->ARR = 499u;
    TIM2->EGR = TIM_EGR_UG; /* Load prescaler before starting. */
    TIM2->SR = 0u;
    TIM2->CNT = 0u;
    latest_adc = 0u;
    latest_dac = 0u;
    requested_coarse = false;
    latest_coarse = false;
    sample_valid = false;
    sampling_fault = false;
    sample_count = 0u;
    NVIC_ClearPendingIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 1u);
    TIM2->DIER = TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM2_IRQn);
    TIM2->CR1 = TIM_CR1_CEN;
    return true;
}

void sampling_set_coarse(bool coarse)
{
    requested_coarse = coarse; /* Atomic byte store on this MCU. */
}

void sampling_snapshot(sampling_snapshot_t *out)
{
    if (out == NULL) return;
    /* Take one consistent ADC/DAC/mode pair; preserve the caller's mask. */
    uint32_t saved_primask = __get_PRIMASK();
    __disable_irq();
    out->adc_code = latest_adc;
    out->dac_code = latest_dac;
    out->coarse = latest_coarse;
    out->valid = sample_valid;
    out->fault = sampling_fault;
    __set_PRIMASK(saved_primask);
}

void sampling_timer_irq(void)
{
    if ((TIM2->SR & TIM_SR_UIF) == 0u) return;
    TIM2->SR = 0u; /* This module owns TIM2 and enables only update events. */
    uint16_t adc;
    bool coarse = requested_coarse;
    if (!voltage_mirror_read(&adc))
    {
        TIM2->CR1 &= ~TIM_CR1_CEN;
        TIM2->DIER = 0u;
        sampling_fault = true;
        return; /* Leave the DAC at the last successfully written code. */
    }
    uint16_t dac = voltage_mirror_map(adc, coarse);
    voltage_mirror_write(dac);
    latest_adc = adc;
    latest_dac = dac;
    latest_coarse = coarse;
    sample_valid = true;
    ++sample_count;
}
