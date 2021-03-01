#include "modules/dyn/dyn.h"
#include "common.h"
#include "utilities.h"

namespace fk {

void fkb_log_header(fkb_header_t const *fkbh) {
    alogf(LogLevels::INFO, "fkb", "[0x%8p] found '%s' / #%" PRIu32 " '%s' flags=0x%" PRIx32 " size=%" PRIu32 " dyntables=+%" PRIu32 " data=%" PRIu32 " bss=%" PRIu32 " got=%" PRIu32 " vtor=0x%" PRIx32, fkbh,
            fkbh->firmware.name, fkbh->firmware.number, fkbh->firmware.version,
            fkbh->firmware.flags, fkbh->firmware.binary_size, fkbh->firmware.tables_offset,
            fkbh->firmware.data_size, fkbh->firmware.bss_size, fkbh->firmware.got_size,
            fkbh->firmware.vtor_offset);

    char hex_hash[(fkbh->firmware.hash_size * 2) + 1];
    bytes_to_hex_string(hex_hash, sizeof(hex_hash), fkbh->firmware.hash, fkbh->firmware.hash_size);

    alogf(LogLevels::INFO, "fkb", "[0x%8p] hash='%s' timestamp=%" PRIu32, fkbh, hex_hash, fkbh->firmware.timestamp);
}

bool fkb_same_header(fkb_header_t const *a, fkb_header_t const *b) {
    if (a == nullptr || b == nullptr) return false;
    if (a->firmware.hash_size != b->firmware.hash_size) return false;
    return memcmp(a->firmware.hash, b->firmware.hash, b->firmware.hash_size) == 0;
}

} // namespace fk

#if !defined(__SAMD51__)

extern "C" {

void fk_dyn_run(void *got, void *entry) {
}

}

#endif
