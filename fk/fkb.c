/**
 *
 *
 *
 */
#include <loading.h>

#define FK_XSTR(s) FK_STR(s)

#define FK_STR(s)  #s

__attribute__((section(".fkb.header")))
const struct fkb_header_t fkb_header = {
    .signature          = FKB_HEADER_SIGNATURE(),
    .version            = 1,
    .size               = sizeof(fkb_header_t),
    .firmware           = {
        .flags          = 0,
        .timestamp      = 0,
        .number         = 0,
        .reserved       = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
        .safe           = 0xff,
        .previous       = UINT32_MAX,
        .binary_size    = 0,
        .tables_offset  = 0,
        .data_size      = 0,
        .bss_size       = 0,
        .got_size       = 0,
        .vtor_offset    = 0,
        .got_offset     = 0,
        .version        = FK_XSTR(FK_VERSION),
        .hash_size      = 0,
        .hash           = { 0 }
    },
    .number_symbols     = 0,
    .number_relocations = 0
};
