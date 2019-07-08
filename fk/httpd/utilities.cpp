#include "common.h"
#include "printf.h"

namespace fk {

size_t bytes_to_hex_string(char *buffer, size_t buffer_size, const uint8_t *data, size_t data_size) {
    char *s = buffer;
    FK_ASSERT(buffer_size >= data_size * 2);

    for (size_t i = 0; i < data_size; ++i) {
        fk_snprintf(s, 3, "%02x", data[i]);
        s += 2;
    }

    return data_size * 2;
}

}
