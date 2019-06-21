/**
 *
 *
 *
 */
#include <loading.h>

__attribute__((section(".fkbh")))
const struct fkb_header_t fkb_header = {
    .signature          = FKB_HEADER_SIGNATURE(),
    .version            = 1,
    .size               = sizeof(fkb_header_t),
    .firmware           = {
        .flags          = 0,
        .timestamp      = 0,
        .binary_size    = 0,
        .vtor_offset    = 0,
        .name           = "",
        .hash_size      = 0,
        .hash           = ""
    }
};
