#include <tiny_printf.h>

#include "utilities.h"

namespace fk {

size_t bytes_to_hex_string(char *buffer, size_t buffer_size, const uint8_t *data, size_t data_size) {
    char *s = buffer;
    FK_ASSERT(buffer_size >= data_size * 2);

    for (size_t i = 0; i < data_size; ++i) {
        tiny_snprintf(s, 3, "%02x", data[i]);
        s += 2;
    }

    return data_size * 2;
}

const char *bytes_to_hex_string_pool(const uint8_t *data, size_t data_size, Pool &pool) {
    auto str = (char *)pool.malloc(data_size * 2);
    bytes_to_hex_string(str, data_size * 2, data, data_size);
    return str;
}

}
