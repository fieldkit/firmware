#include <loading.h>

#include "modules/dyn/process.h"
#include "modules/dyn/compiled.h"

#include "utilities.h"
#include "syscalls_app.h"

#include "hal/memory.h"

#if defined(__SAMD51__)

extern "C" {

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

static syscall_entry_t externals[] = {
    { "fkos_delay",  (void *)os_delay },
    { "fkos_logf", (void *)fkos_logf },
};

static constexpr size_t externals_size = sizeof(externals) / sizeof(syscall_entry_t);

namespace fk {

FK_DECLARE_LOGGER("proc");

static fkb_symbol_t *get_symbol_by_index(fkb_header_t *header, uint32_t symbol);
static fkb_symbol_t *get_first_symbol(fkb_header_t *header);

static uint32_t allocate_process_got(fkb_header_t *header, uint32_t *got, uint32_t *data) {
    auto base = (uint8_t *)header;

    loginfo("[0x%8p] number-syms=%" PRIu32 " number-rels=%" PRIu32 "got=0x%" PRIx32, base,
            header->number_symbols, header->number_relocations,
            header->firmware.got_offset);

    auto sym = get_first_symbol(header);
    for (auto i = 0u; i < header->number_symbols; ++i) {
        auto ptr = (uint32_t *)((uint8_t *)got + sym->address);
        auto linked = false;

        #if defined(__SAMD51__)
        for (auto i = 0u; i < externals_size; ++i) {
            if (strncmp((const char *)sym->name, externals[i].name, sizeof(sym->name)) == 0) {
                // Notice this address taking and remember that the
                // GOT has pointers to function pointers!
                *ptr = (uint32_t)&externals[i].fn;
                linked = true;
                break;
            }
        }

        if (!linked) {
            *ptr = (uint32_t)(void *)data;
            data += (sym->size / 4);
        }
        #endif

        loginfo("[0x%8p] %s addr=0x%8" PRIx32 " size=0x%4" PRIx32 " '%s' (0x%8p = 0x%" PRIx32 ")",
                base, linked ? "linked" : "alloc", sym->address, sym->size, sym->name, ptr, *ptr);

        sym++;
    }

    return 0;
}

static void log_fkb_header(fkb_header_t const *fkbh) {
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
    auto memory = MemoryFactory::get_qspi_memory();

    loginfo("begin");

    if (!memory->begin()) {
        return;
    }

    loginfo("ready");

    #if 1
    auto header = (fkb_header_t *)0x04000000;
    #else
    auto header = (fkb_header_t *)build_samd51_modules_dynamic_main_fkdynamic_fkb_bin;
    #endif

    log_fkb_header(header);

    auto got = (uint32_t *)pool.malloc(header->firmware.got_size);
    auto data = (uint32_t *)pool.malloc(header->firmware.data_size);

    if (allocate_process_got(header, got, data)) {
        logerror("error allocating got");
        return;
    }

    // Notice the +1 here is intentional, per the ARM standard.
    auto entry = (uint32_t *)(((uint8_t *)header) + header->firmware.vtor_offset + 1);

    loginfo("calling module");

    memory->execute(got, entry);

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

#endif
