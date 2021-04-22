#include <tiny_printf.h>

#include "bl.h"

extern "C" {

void bl_fkb_log_header(fkb_header_t const *fkbh) {
    if (!fkb_has_valid_signature(fkbh)) {
        fkb_external_println("bl: [0x%8p] invalid header", fkbh);
        return;
    }

    fkb_external_println("bl: [0x%8p] found '%s' (#%" PRIu32 ") flags=0x%" PRIx32 " timestamp=%" PRIu32 " previous=%" PRIu32 " binary-size=%" PRIu32 " dyntables=+%" PRIu32 " data=%" PRIu32 " bss=%" PRIu32 " got=%" PRIu32 " vtor=0x%" PRIx32,
                         fkbh, fkbh->firmware.version, fkbh->firmware.number, fkbh->firmware.flags,
                         fkbh->firmware.binary_size, fkbh->firmware.tables_offset, fkbh->firmware.data_size,
                         fkbh->firmware.bss_size, fkbh->firmware.got_size, fkbh->firmware.vtor_offset);

    if (fkbh->firmware.hash_size < 128) {
        char hex_hash[(fkbh->firmware.hash_size * 2) + 1];
        fkb_bytes_to_hex(hex_hash, sizeof(hex_hash), fkbh->firmware.hash, fkbh->firmware.hash_size);

        fkb_external_println("bl: [0x%8p] hash='%s' timestamp=%" PRIu32, fkbh, hex_hash, fkbh->firmware.timestamp);
    }
}

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

}
