#pragma once

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

int32_t bl_flash_initialize();

int32_t bl_flash_read(uint32_t address, uint8_t *data, int32_t size);

int32_t bl_flash_write(uint32_t address, uint8_t const *data, int32_t size);

int32_t bl_flash_erase(uint32_t address, int32_t size);

extern uint32_t bl_flash_page_size;

extern uint32_t bl_flash_total_pages;

inline uint32_t aligned_on(uint32_t value, uint32_t on) {
    return ((value % on != 0) ? (value + (on - (value % on))) : value);
}

#if defined(__cplusplus)
} // extern "C"
#endif
