#pragma once

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

void bl_dump_memory(const char *prefix, const uint8_t *p, size_t size, ...);

void bl_fkb_log_header(fkb_header_t const *fkbh);

#if defined(__cplusplus)
} // extern "C"
#endif
