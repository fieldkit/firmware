#include <tiny_printf.h>

#include "bl.h"

extern "C" {

void bl_dump_memory(const char *prefix, const uint8_t *p, size_t size, ...) {
    va_list args;
    va_start(args, size);

    #if defined(__SAMD51__)
    SEGGER_RTT_LOCK();
    #endif
    fkb_external_vprintf(prefix, args);
    for (auto i = (size_t)0; i < size; ++i) {
        fkb_external_printf("%02x ", p[i]);
        if ((i + 1) % 32 == 0) {
            if (i + 1 < size) {
                fkb_external_printf("\n");
                fkb_external_vprintf(prefix, args);
            }
        }
    }
    fkb_external_printf(" (%d bytes)\n", size);
#if defined(__SAMD51__)
    SEGGER_RTT_UNLOCK();
#endif

    va_end(args);
}

/**
 * Write hex representation of bytes to a separate buffer.
 */
size_t bytes_to_hex_string(char *buffer, size_t buffer_size, uint8_t const *data, size_t data_size);

size_t bytes_to_hex_string(char *buffer, size_t buffer_size, const uint8_t *data, size_t data_size) {
    auto length = data_size * 2;
    char *s = buffer;

    for (size_t i = 0; i < data_size; ++i) {
        tiny_snprintf(s, 3, "%02x", data[i]);
        s += 2;
    }

    s[0] = 0;

    return length;
}

void fkb_log_header(fkb_header_t const *fkbh) {
    if (memcmp(fkbh->signature, FKB_HEADER_SIGNATURE(), sizeof(fkbh->signature)) != 0) {
        fkb_external_println("[0x%8p] invalid header", fkbh);
        return;
    }

    fkb_external_println("[0x%8p] found '%s' / #%" PRIu32 " '%s' flags=0x%" PRIx32 " size=%" PRIu32 " dyntables=+%" PRIu32 " data=%" PRIu32 " bss=%" PRIu32 " got=%" PRIu32 " vtor=0x%" PRIx32, fkbh,
            fkbh->firmware.name, fkbh->firmware.number, fkbh->firmware.version,
            fkbh->firmware.flags, fkbh->firmware.binary_size, fkbh->firmware.tables_offset,
            fkbh->firmware.data_size, fkbh->firmware.bss_size, fkbh->firmware.got_size,
            fkbh->firmware.vtor_offset);

    if (fkbh->firmware.hash_size < 128) {
        char hex_hash[(fkbh->firmware.hash_size * 2) + 1];
        bytes_to_hex_string(hex_hash, sizeof(hex_hash), fkbh->firmware.hash, fkbh->firmware.hash_size);

        fkb_external_println("[0x%8p] hash='%s' timestamp=%" PRIu32, fkbh, hex_hash, fkbh->firmware.timestamp);
    }
}

int32_t fkb_same_header(fkb_header_t const *a, fkb_header_t const *b) {
    if (a == nullptr || b == nullptr) return false;
    if (a->firmware.hash_size != b->firmware.hash_size) return false;
    return memcmp(a->firmware.hash, b->firmware.hash, b->firmware.hash_size) == 0;
}

}
