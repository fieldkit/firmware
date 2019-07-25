#pragma once

#include <inttypes.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 */
uint32_t crc32_update(uint32_t crc, uint8_t data);

/**
 *
 */
uint32_t crc32_checksum(uint8_t *data, size_t size);


#ifdef __cplusplus
}
#endif
