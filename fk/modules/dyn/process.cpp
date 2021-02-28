#include <loading.h>

#include "modules/dyn/process.h"
#include "modules/dyn/compiled.h"

#include "utilities.h"
#include "syscalls_app.h"

#if defined(__SAMD51__)

extern "C" {

void fk_dyn_run(void *got, void *entry);

static size_t fkos_logf(uint32_t level, const char *facility, const char *f, ...) {
    va_list args;
    va_start(args, f);
    valogf((LogLevels)level, facility, f, args);
    va_end(args);
    return 0;
}

}

typedef struct syscall_entry_t {
    const char *name;
    void *fn;
} syscall_entry_t;

static constexpr size_t externals_size = 2;

static syscall_entry_t externals[externals_size] = {
    { "fkos_delay",  (void *)os_delay },
    { "fkos_logf", (void *)fkos_logf },
};

#else

extern "C" void fk_dyn_run(void *got, void *entry) {
}

#endif

namespace fk {

FK_DECLARE_LOGGER("proc");

static fkb_symbol_t *get_symbol_by_index(fkb_header_t *header, uint32_t symbol);
static fkb_symbol_t *get_first_symbol(fkb_header_t *header);

static uint32_t allocate_process_got(fkb_header_t *header, uint8_t *got, uint8_t *data) {
    auto base = (uint8_t *)header;
    auto syms = get_first_symbol(header);

    fkb_external_println("bl: [0x%08x] number-syms=%d number-rels=%d got=0x%x", base,
                         header->number_symbols, header->number_relocations,
                         header->firmware.got_offset);
    fkb_external_println("bl: [0x%08x] first-sym=0x%x", base, syms);

    auto s = syms;
    for (auto i = 0u; i < header->number_symbols; ++i) {
        auto ptr = (uint32_t *)((uint8_t *)got + s->address);
        auto linked = false;

        #if defined(__SAMD51__)
        for (auto i = 0u; i < externals_size; ++i) {
            if (strncmp((const char *)s->name, externals[i].name, sizeof(s->name)) == 0) {
                // Notice this address taking and remember that the
                // GOT has pointers to function pointers!
                *ptr = (uint32_t)&externals[i].fn;
                linked = true;
                break;
            }
        }

        if (!linked) {
            *ptr = (uint32_t)(void *)data;
            data += s->size;
        }
        #endif

        fkb_external_println("bl: [0x%08x] %s #%6d addr=0x%8x size=0x%4x '%s' (0x%x = 0x%x)",
                             base, linked ? "linked" : "alloc", i, s->address, s->size, s->name, ptr, *ptr);

        s++;
    }

    return 0;
}


static void log_fkb_header(fkb_header_t *fkbh) {
    loginfo("[0x%8p] found '%s' / #%" PRIu32 " '%s' flags=0x%" PRIx32 " size=%" PRIu32 " dyntables=+%" PRIu32 " data=%" PRIu32 " bss=%" PRIu32 " got=%" PRIu32 " vtor=0x%" PRIx32, fkbh,
            fkbh->firmware.name, fkbh->firmware.number, fkbh->firmware.version,
            fkbh->firmware.flags, fkbh->firmware.binary_size, fkbh->firmware.tables_offset,
            fkbh->firmware.data_size, fkbh->firmware.bss_size, fkbh->firmware.got_size,
            fkbh->firmware.vtor_offset);

    char hex_hash[(fkbh->firmware.hash_size * 2) + 1];
    bytes_to_hex_string(hex_hash, sizeof(hex_hash), fkbh->firmware.hash, fkbh->firmware.hash_size);

    loginfo("[0x%8p] hash='%s' timestamp=%" PRIu32, fkbh, hex_hash, fkbh->firmware.timestamp);
}

void Process::run(Pool &pool) {
    auto header = (fkb_header_t *)build_samd51_modules_dynamic_main_fkdynamic_fkb_bin;

    log_fkb_header(header);

    auto got = (uint8_t *)pool.malloc(header->firmware.got_size);
    auto data = (uint8_t *)pool.malloc(header->firmware.data_size);

    if (allocate_process_got(header, got, data)) {
        logerror("error allocating got");
        return;
    }

    // Notice the +1 here is intentional, per the ARM standard.
    auto entry = ((uint8_t *)header) + header->firmware.vtor_offset + 1;

    loginfo("calling module");

    fk_dyn_run(got, entry);

    loginfo("module finished");

    while (true) {
        os_delay(1000);
    }
}

static fkb_symbol_t *get_symbol_by_index(fkb_header_t *header, uint32_t symbol) {
    uint8_t *base = (uint8_t *)header + header->firmware.tables_offset;
    return (fkb_symbol_t *)(base + sizeof(fkb_symbol_t) * symbol);
}

static fkb_symbol_t *get_first_symbol(fkb_header_t *header) {
    return get_symbol_by_index(header, 0);
}

} // namespace fk
