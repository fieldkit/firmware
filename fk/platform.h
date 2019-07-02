#pragma once

#include "common.h"

namespace fk {

/**
 * Return the number of milliseconds since the hardware was started.
 */
uint32_t fk_uptime();

uint32_t fk_delay(uint32_t ms);

uint32_t fk_free_memory();

}

#ifdef __cplusplus
extern "C" {
#endif

uint32_t fkb_external_printf(const char *str, ...) __attribute__((format(printf, 1, 2)));

uint32_t fkb_external_println(const char *str, ...) __attribute__((format(printf, 1, 2)));

#ifdef __cplusplus
}
#endif
