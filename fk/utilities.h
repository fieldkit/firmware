#pragma once

#include "common.h"
#include "pool.h"

namespace fk {

/**
 * Write hex representation of bytes to a separate buffer.
 */
size_t bytes_to_hex_string(char *buffer, size_t buffer_size, uint8_t const *data, size_t data_size);

/**
 * Write hex representation of bytes to a separate buffer.
 */
const char *bytes_to_hex_string_pool(uint8_t const *data, size_t data_size, Pool &pool);

/**
 * Function for dumping memory in hexadecimal encoding to the console for debugging.
 */
void fk_dump_memory(const char *prefix, uint8_t const *p, size_t size);

/**
 * Reverse a bytes to hex string, generating bytes from a hex string.
 */
size_t hex_string_to_bytes(uint8_t *data, size_t data_size, const char *buffer);

/**
 *
 */
size_t make_pretty_time_string(uint32_t ms, char *buffer, size_t buffer_size);

}
