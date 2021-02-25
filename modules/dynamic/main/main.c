#include <inttypes.h>
#include <loading.h>

uint32_t os_delay(uint32_t ms);

size_t fkos_logf(uint8_t level, const char *facility, const char *f, ...) __attribute__((unused)) __attribute__((format(printf, 3, 4)));

int32_t fkmodule() {
    while (1) {
        os_delay(1000);
    }

    return 0;
}

__attribute__((section(".fkb.header")))
const struct fkb_header_t fkb_header = {
    .signature          = FKB_HEADER_SIGNATURE(),
    .version            = 1,
    .size               = sizeof(fkb_header_t),
    .firmware           = {
        .flags          = 0,
        .timestamp      = 0,
        .binary_size    = 0,
        .vtor_offset    = 0,
        .got_offset     = 0,
        .name           = { 0 },
        .hash_size      = 0,
        .hash           = { 0 }
    },
    .number_symbols     = 0,
    .number_relocations = 0
};
