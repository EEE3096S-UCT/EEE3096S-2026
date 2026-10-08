/* Core exception handlers and the HAL millisecond time base. */
#include "main.h"
#include "stm32f0xx_it.h"
#include "sampling.h"

void NMI_Handler(void)
{
    Error_Handler();
}

void HardFault_Handler(void)
{
    Error_Handler();
}

void SVC_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}

void TIM2_IRQHandler(void)
{
    sampling_timer_irq();
}
