#ifndef PRACTICAL3_VOLTAGE_MIRROR_H
#define PRACTICAL3_VOLTAGE_MIRROR_H

#include <stdbool.h>
#include <stdint.h>

#define MIRROR_MAX_CODE 4095U

/* Both hardware converters stay at 12 bits. Only the digital mapping changes.
 * The supplied timer calls read/map/write at 2000 samples per second.
 */
bool voltage_mirror_init(void);
bool voltage_mirror_read(uint16_t *code);
void voltage_mirror_write(uint16_t code);
uint16_t voltage_mirror_map(uint16_t code, bool coarse);

#endif
