#include <inttypes.h>
#include <loading.h>

/**
 * These pointers are weird and unfortunately necessary.
 *
 * GCC was making PLTs that weren't using R9 to access the
 * GOT. Supposedly there is a flag -fno-plt that forces those
 * accesses to go through the GOT, though that's not implemented.
 *
 * The workaround is to just use function pointers, which get looked
 * up via the GOT in the usual manner for data.
 *
 * https://answers.launchpad.net/gcc-arm-embedded/+question/669758
 *
 */
extern uint32_t (*os_delay)(uint32_t ms);

extern size_t (*fkos_logf)(uint8_t level, const char *facility, const char *f, ...) __attribute__((unused)) __attribute__((format(printf, 3, 4)));

uint32_t counter = 0;

int32_t fkmodule() {
    while (1) {
        os_delay(1000);
        counter++;
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

__attribute__((section(".fkdyn")))
const uint8_t data[0x1000] = { 0 };
