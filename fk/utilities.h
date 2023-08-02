#pragma once

#include "common.h"
#include "pool.h"
#include "ethutil.h"

namespace fk {

size_t log_bytes(const char *prefix, uint8_t const *ptr, size_t size);

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
void fk_dump_memory(const char *prefix, uint8_t const *p, size_t size, ...);

/**
 * Reverse a bytes to hex string, generating bytes from a hex string.
 */
size_t hex_string_to_bytes(uint8_t *data, size_t data_size, const char *buffer);

/**
 *
 */
size_t make_pretty_time_string(uint32_t ms, char *buffer, size_t buffer_size);

/**
 *
 */
inline uint32_t aligned_on(uint32_t value, uint32_t on) {
    return ((value % on != 0) ? (value + (on - (value % on))) : value);
}

/**
 *
 */
template <typename T> int32_t for_each_block_between(uint32_t address, size_t length, size_t block_size, T fn) {
    auto aligned = aligned_on(length, block_size);
    for (auto e = 0u; e < aligned; e += block_size) {
        auto err = fn(address + e);
        if (err < 0) {
            return err;
        }
    }
    return 0;
}

bool is_null_byte_array(uint8_t const *ptr, size_t length);

#if defined(__SAMD51__)

/**
 *
 */
inline uint8_t const *aligned_on(uint8_t const *value, uint32_t on) {
    return (uint8_t const *)aligned_on((uint32_t)value, on);
}

#else

#endif

} // namespace fk
