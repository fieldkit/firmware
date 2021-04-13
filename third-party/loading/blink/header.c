#include <loading.h>

#if defined(FKB_ENABLE_HEADER)

__attribute__((section(".fkb.header")))
const fkb_header_t fkb_header = {
    .signature          = FKB_HEADER_SIGNATURE(),
    .version            = 1,
    .size               = sizeof(fkb_header_t),
    .firmware           = {
        .flags          = 0,
        .timestamp      = 0,
        .binary_size    = 0,
        .vtor_offset    = 0,
        .got_offset     = 0,
        .version        = { 0 },
        .hash_size      = 0,
        .hash           = { 0 }
    },
    .number_symbols     = 0,
    .number_relocations = 0
};

#endif

__attribute__((section(".fkb.launch")))
fkb_launch_info_t fkb_launch_info = {
    .upgrading = 0,
};
