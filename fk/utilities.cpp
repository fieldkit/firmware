#include <tiny_printf.h>

#if defined(__SAMD51__)
#include <SEGGER_RTT.h>
#endif

#include <loading.h>

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

void fk_dump_memory(const char *prefix, const uint8_t *p, size_t size) {
    #if defined(__SAMD51__)
    SEGGER_RTT_LOCK();
    #endif
    fkb_external_printf("%s", prefix);
    for (auto i = (size_t)0; i < size; ++i) {
        fkb_external_printf("%02x ", p[i]);
        if ((i + 1) % 32 == 0) {
            if (i + 1 < size) {
                fkb_external_printf("\n%s", prefix);
            }
        }
    }
    fkb_external_printf(" (%d bytes)\n", size);
    #if defined(__SAMD51__)
    SEGGER_RTT_UNLOCK();
    #endif
}

}
