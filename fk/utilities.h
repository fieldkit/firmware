#pragma once

#include "common.h"
#include "pool.h"

namespace fk {

/**
 * Write hex representation of bytes to a separate buffer.
 */
size_t bytes_to_hex_string(char *buffer, size_t buffer_size, const uint8_t *data, size_t data_size);

/**
 * Write hex representation of bytes to a separate buffer.
 */
const char *bytes_to_hex_string_pool(const uint8_t *data, size_t data_size, Pool &pool);

}