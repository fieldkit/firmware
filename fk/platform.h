#pragma once

#include "common.h"

namespace fk {

/**
 * Returnt he number of milliseconds since the hardware was started.
 */
uint32_t fk_uptime();

}

#ifdef __cplusplus
extern "C" {
#endif

uint32_t fkb_external_printf(const char *str, ...);

uint32_t fkb_external_println(const char *str, ...);

#ifdef __cplusplus
}
#endif
