/* Exhaustive range, precision and information-loss checks on the actual map. */
#include "voltage_mirror.h"
#include "main.h"
#include <assert.h>
#include <stdio.h>

ADC_TypeDef mock_adc;
DAC_TypeDef mock_dac;
bool board_converters_prepare(void) { return false; }
bool board_adc_enable(void) { return false; }

int main(void)
{
    unsigned distinct = 0U;
    uint16_t previous = 0U;
    for (unsigned input = 0U; input <= MIRROR_MAX_CODE; ++input)
    {
        uint16_t full = voltage_mirror_map((uint16_t)input, false);
        uint16_t coarse = voltage_mirror_map((uint16_t)input, true);
        assert(full == input);
        assert(coarse <= input);
        assert(input - coarse < 64U);
        assert(coarse % 64U == 0U);
        if (input == 0U || coarse != previous)
        {
            if (input != 0U) assert(coarse - previous == 64U);
            ++distinct;
        }
        previous = coarse;
    }
    assert(distinct == 64U);
    assert(previous == 4032U);
    for (unsigned input = MIRROR_MAX_CODE + 1U; input <= UINT16_MAX; ++input)
    {
        assert(voltage_mirror_map((uint16_t)input, false) == MIRROR_MAX_CODE);
        assert(voltage_mirror_map((uint16_t)input, true) == 4032U);
    }
    const uint16_t commands[] = {0U, 2048U, 4095U, 4096U, UINT16_MAX};
    for (unsigned i = 0U; i < sizeof commands / sizeof commands[0]; ++i)
    {
        voltage_mirror_write(commands[i]);
        assert(mock_dac.DHR12R1 == (commands[i] > 4095U ? 4095U : commands[i]));
    }
    puts("PASS: all 4096 codes, 64 coarse levels, saturation, and bounded DAC writes");
    return 0;
}
