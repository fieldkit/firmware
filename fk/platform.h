#pragma once

#include "common.h"

#if !defined(__SAMD51__)
#include <vector>
#endif

namespace fk {

typedef struct fk_serial_number_t {
    uint32_t dwords[4];

    fk_serial_number_t();
} fk_serial_number_t;

/**
 * Return the number of milliseconds since the hardware was started.
 */
uint32_t fk_uptime();

uint32_t fk_delay(uint32_t ms);

uint32_t fk_uptime_adjust_after_sleep(uint32_t delta);

uint32_t fk_deep_sleep(uint32_t ms);

uint32_t fk_free_memory();

uint32_t fk_serial_number_get(fk_serial_number_t *sn);

uint32_t fk_task_self_priority_get();

uint32_t fk_task_self_priority_set(uint32_t priority);

#if !defined(__SAMD51__)
uint32_t fk_fake_uptime(std::vector<uint32_t> more);
#endif

} // namespace fk
