#ifndef MIRROR_TEST_MAIN_H
#define MIRROR_TEST_MAIN_H

#include <stddef.h>
#include <stdint.h>

/* Ordinary host variables, only to compile the actual mapping. They do not
 * simulate W1C flags, conversion, peripheral timing or analogue IO.
 */
typedef struct { uint32_t CR, CFGR1, CHSELR, SMPR, ISR, DR; } ADC_TypeDef;
typedef struct { uint32_t DHR12R1, CR; } DAC_TypeDef;
extern ADC_TypeDef mock_adc;
extern DAC_TypeDef mock_dac;
#define ADC1 (&mock_adc)
#define DAC (&mock_dac)
#define ADC_CHSELR_CHSEL5 (1U << 5)
#define ADC_SMPR_SMP 7U
#define DAC_CR_EN1 1U
#define ADC_CR_ADEN 1U
#define ADC_CR_ADSTART (1U << 2)
#define ADC_CR_ADSTP (1U << 4)
#define ADC_ISR_EOC (1U << 2)
#define ADC_ISR_EOS (1U << 3)
#define ADC_ISR_OVR (1U << 4)

#endif
