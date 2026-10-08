#ifndef PRACTICAL3_SAMPLING_H
#define PRACTICAL3_SAMPLING_H

#include <stdbool.h>
#include <stdint.h>

#define SAMPLING_RATE_HZ 2000u

typedef struct
{
    uint16_t adc_code;
    uint16_t dac_code;
    bool coarse;             /* Mode used for this pair of codes. */
    bool valid;              /* True only after an actual conversion. */
    bool fault;              /* Acquisition stopped after a failure. */
} sampling_snapshot_t;

/* Supplied timing: start only after voltage_mirror_init() succeeds. */
bool sampling_start(void);
void sampling_set_coarse(bool coarse);
void sampling_snapshot(sampling_snapshot_t *out);
void sampling_timer_irq(void);

#endif /* PRACTICAL3_SAMPLING_H */
