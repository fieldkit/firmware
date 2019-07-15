#pragma once

#include "common.h"

typedef struct fk_serial_number_t {
    uint32_t dwords[4];
} fk_serial_number_t;

#if !defined(__SAMD51__)
#include <vector>
#endif

namespace fk {

/**
 * Return the number of milliseconds since the hardware was started.
 */
uint32_t fk_uptime();

uint32_t fk_delay(uint32_t ms);

uint32_t fk_free_memory();

#if !defined(__SAMD51__)
uint32_t fk_fake_uptime(std::vector<uint32_t> more);
#endif

uint32_t fk_serial_number_get(fk_serial_number_t *sn);

void fk_dump_memory(uint8_t *p, size_t size);

}
