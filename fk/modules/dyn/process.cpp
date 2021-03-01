#include <loading.h>

#include "modules/dyn/process.h"
#include "modules/dyn/compiled.h"
#include "modules/dyn/dyn.h"

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

        loginfo("[0x%8p] %-6s addr=0x%8" PRIx32 " size=0x%4" PRIx32 " '%s' (0x%8p = 0x%" PRIx32 ")",
                base, linked ? "linked" : "alloc", sym->address, sym->size, sym->name, ptr, *ptr);

        sym++;
    }

    return 0;
}

static uint32_t aligned_on(uint32_t value, uint32_t on) {
    return ((value % on != 0) ? (value + (on - (value % on))) : value);
}

static uint8_t const *aligned_on(uint8_t const *value, uint32_t on) {
    return (uint8_t const *)aligned_on((uint32_t)value, on);
}

class FirmwareStorage {
public:
    template<typename R>
    optional<R> walk(std::function<optional<R> (fkb_header_t const *)> fn) {
        auto ptr = begin();

        while (ptr < end()) {
            loginfo("[0x%8p] checking", ptr);
            auto fkbh = fkb_try_header(ptr);
            if (fkbh == nullptr) {
                ptr = aligned_on(ptr, block_size());
                loginfo("[0x%8p] checking", ptr);
                fkbh = fkb_try_header(ptr);
                if (fkbh == nullptr) {
                    break;
                }
            }

            optional<R> r = fn(fkbh);
            if (r) {
                return r;
            }

            FK_ASSERT(fkbh->firmware.binary_size > 0);

            ptr += fkbh->firmware.binary_size;
        }

        return nullopt;
    }

public:
    optional<fkb_header_t const *> find(fkb_header_t const *needle) {
        return walk<fkb_header_t const *>([=](fkb_header_t const *fkbh) -> optional<fkb_header_t const *> {
            if (fkb_same_header(fkbh, needle)) {
                return fkbh;
            }
            return nullopt;
        });
    }

    optional<uint32_t> find(size_t required) {
        return nullopt;
    }

public:
    uint8_t const *begin() const {
        return (uint8_t *)0x04000000;
    }

    uint8_t const *end() const {
        return begin() + (1024 * 1024 * 1);
    }

    size_t block_size() const {
        return 0x10000;
    }

    size_t minimum_allocation_size() const {
        return block_size();
    }

};

void Process::run(Pool &pool) {
    auto memory = MemoryFactory::get_qspi_memory();

    loginfo("begin");

    if (!memory->begin()) {
        return;
    }

    loginfo("ready");

    FirmwareStorage firmware;

    if (!firmware.walk<bool>([](fkb_header_t const *fkbh) {
        fkb_log_header(fkbh);
        return true;
    })) {
        return;
    }

    auto incoming = (fkb_header_t *)build_samd51_modules_dynamic_main_fkdynamic_fkb_bin;
    auto found = firmware.find(incoming);
    if (!found) {
        loginfo("missing firmware, finding room");
        firmware.find(incoming->size);
    }

    #if 1
    auto header = (fkb_header_t *)0x04000000;
    #else
    auto header = (fkb_header_t *)build_samd51_modules_dynamic_main_fkdynamic_fkb_bin;
    #endif

    // log_fkb_header(header);

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
