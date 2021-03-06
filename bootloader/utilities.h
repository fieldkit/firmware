#pragma once

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

void bl_dump_memory(const char *prefix, const uint8_t *p, size_t size, ...);

void fkb_log_header(fkb_header_t const *fkbh);

int32_t fkb_same_header(fkb_header_t const *a, fkb_header_t const *b);

#if defined(__cplusplus)
} // extern "C"
#endif
