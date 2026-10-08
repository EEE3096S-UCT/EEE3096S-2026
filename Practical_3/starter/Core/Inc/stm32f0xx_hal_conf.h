#ifndef PRACTICAL3_STM32F0XX_HAL_CONF_H
#define PRACTICAL3_STM32F0XX_HAL_CONF_H

/* HAL is used for clock and millisecond infrastructure. ADC/DAC work uses
 * direct registers; those HAL modules are disabled.
 */
#define HAL_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED

#ifndef HSE_VALUE
#define HSE_VALUE 8000000U
#endif
#ifndef HSE_STARTUP_TIMEOUT
#define HSE_STARTUP_TIMEOUT 100U
#endif
#ifndef HSI_VALUE
#define HSI_VALUE 8000000U
#endif
#ifndef HSI_STARTUP_TIMEOUT
#define HSI_STARTUP_TIMEOUT 5000U
#endif
#ifndef HSI14_VALUE
#define HSI14_VALUE 14000000U
#endif
#ifndef HSI48_VALUE
#define HSI48_VALUE 48000000U
#endif
#ifndef LSI_VALUE
#define LSI_VALUE 40000U
#endif
#ifndef LSE_VALUE
#define LSE_VALUE 32768U
#endif
#ifndef LSE_STARTUP_TIMEOUT
#define LSE_STARTUP_TIMEOUT 5000U
#endif

#define VDD_VALUE 3300U
#define TICK_INT_PRIORITY 3U
#define USE_RTOS 0U
#define PREFETCH_ENABLE 1U
#define INSTRUCTION_CACHE_ENABLE 0U
#define DATA_CACHE_ENABLE 0U

#include "stm32f0xx_hal_rcc.h"
#include "stm32f0xx_hal_cortex.h"
#include "stm32f0xx_hal_flash.h"
#include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal_pwr.h"

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line);
#define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
#else
#define assert_param(expr) ((void)0U)
#endif

#endif /* PRACTICAL3_STM32F0XX_HAL_CONF_H */
