#ifndef PRACTICAL3_BOARD_IO_H
#define PRACTICAL3_BOARD_IO_H

#include <stdbool.h>
#include <stdint.h>

/* Supplied board support. SW0..SW3 are active-low inputs on PA0..PA3.
 * Hold SW3 while resetting to exchange LCD RS/E, then release it.
 */
void board_io_init(void);
uint8_t board_buttons_poll(uint32_t now_ms);
void board_lcd_show(const char *line1, const char *line2);
void board_status(bool fault);

/* Enable clocks, put PA4/PA5 in analog mode, reset ADC/DAC, start HSI14,
 * and calibrate the still-disabled ADC. Call once, before ADC setup.
 * After configuring the ADC, board_adc_enable() waits for readiness.
 * Both helpers use bounded waits and return false if hardware fails.
 */
bool board_converters_prepare(void);
bool board_adc_enable(void);

#endif /* PRACTICAL3_BOARD_IO_H */
