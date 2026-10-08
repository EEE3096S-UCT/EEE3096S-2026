/* EEE3096S Practical 3: complete the four TODOs in this file.
 * See ../../../docs/prac_03.md and ST RM0091, ADC and DAC chapters.
 * Use CMSIS register names; clock, GPIO and startup helpers are supplied.
 */
#include "voltage_mirror.h"
#include "board_io.h"
#include "main.h"

bool voltage_mirror_init(void)
{
    if (!board_converters_prepare()) return false;

    /* TODO 1: ADC1: 12-bit, right-aligned, software-started single conversions,
     * channel 5 only, sampling time 239.5 ADC cycles.
     * DAC channel 1: buffer on, trigger off, initial command 2048, enable.
     * Set ADC1->CFGR1, ADC1->CHSELR, ADC1->SMPR, DAC->DHR12R1 and DAC->CR.
     * Finish by returning board_adc_enable(). Remove the false return below.
     */
    return false; /* The uncompleted starter intentionally does not acquire. */
}

bool voltage_mirror_read(uint16_t *code)
{
    /* TODO 2: reject a null pointer; clear EOC/EOS/OVR flags with W1C writes;
     * start one conversion, wait for EOC with a bounded loop, read ADC1->DR.
     * Return true only when *code contains a real acquired 12-bit result.
     * On timeout request ADSTP, then return false. Never wait forever.
     */
    (void)code;
    return false;
}

void voltage_mirror_write(uint16_t code)
{
    /* TODO 3: limit code to 0..4095 and write DAC->DHR12R1. */
    (void)code;
}

uint16_t voltage_mirror_map(uint16_t code, bool coarse)
{
    /* TODO 4: limit code to 0..4095. FULL passes it through; COARSE keeps
     * the six most significant bits and clears the six least significant.
     * Retain the 12-bit DAC scale: do not send a 0..63 code directly.
     */
    (void)coarse;
    return code;
}
