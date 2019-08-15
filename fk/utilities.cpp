#include <tiny_printf.h>

#include "utilities.h"

namespace fk {

size_t bytes_to_hex_string(char *buffer, size_t buffer_size, const uint8_t *data, size_t data_size) {
    auto length = data_size * 2;
    char *s = buffer;
    FK_ASSERT(buffer_size >= length + 1);

    for (size_t i = 0; i < data_size; ++i) {
        tiny_snprintf(s, 3, "%02x", data[i]);
        s += 2;
    }

    s[0] = 0;

    return length;
}

const char *bytes_to_hex_string_pool(const uint8_t *data, size_t data_size, Pool &pool) {
    auto length = data_size * 2 + 1;
    auto str = (char *)pool.malloc(length);
    bytes_to_hex_string(str, length, data, data_size);
    return str;
}

}
