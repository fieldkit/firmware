#include <loading.h>

#include "modules/dyn/process.h"
#include "modules/dyn/compiled.h"
#include "modules/dyn/dyn.h"

#include "utilities.h"
#include "syscalls_app.h"

#include "hal/memory.h"
#include "io.h"
#include "sd_copying.h"

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

static fkb_symbol_t const *get_symbol_by_index(fkb_header_t const *header, uint32_t symbol);

static fkb_symbol_t const *get_first_symbol(fkb_header_t const *header);

uint32_t allocate_process_got(fkb_header_t const *header, uint32_t *got, uint32_t *data) {
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

int32_t fk_module_run(fkb_header_t const *header, Pool &pool) {
    auto got = (uint32_t *)(header->firmware.got_size > 0 ? pool.malloc(header->firmware.got_size) : nullptr);
    auto data = (uint32_t *)(header->firmware.got_size > 0 ? pool.malloc(header->firmware.data_size) : nullptr);

    if (allocate_process_got(header, got, data)) {
        logerror("error allocating got");
        return -1;
    }

    // Notice the +1 here is intentional, per the ARM standard.
    auto entry = (uint32_t *)(((uint8_t *)header) + header->firmware.vtor_offset + 1);

    loginfo("calling module");

    MemoryFactory::get_qspi_memory()->execute(got, entry);

    loginfo("module finished");

    return 0;
}

class FirmwareStorage {
public:
    template<typename R>
    optional<R> walk(std::function<optional<R> (fkb_header_t const *)> fn) {
        auto ptr = begin();

        while (ptr < end()) {
            logverbose("[0x%8p] checking", ptr);
            auto fkbh = fkb_try_header(ptr);
            if (fkbh == nullptr) {
                ptr = aligned_on(ptr, block_size());
                logverbose("[0x%8p] checking", ptr);
                fkbh = fkb_try_header(ptr);
            }
            if (fkbh == nullptr) {
                ptr += FK_MEMORY_BOOTLOADER_SIZE;
                logverbose("[0x%8p] checking", ptr);
                fkbh = fkb_try_header(ptr);
            }

            if (fkbh != nullptr) {
                optional<R> r = fn(fkbh);
                if (r) {
                    return r;
                }

                FK_ASSERT(fkbh->firmware.binary_size > 0);

                ptr += fkbh->firmware.binary_size;
            }
            else {
                ptr += block_size();
            }
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
        return 0;
    }

public:
    uint8_t const *begin() const {
        return (uint8_t *)FK_MEMORY_QSPI_BASE;
    }

    uint8_t const *end() const {
        return begin() + FK_MEMORY_QSPI_SIZE;
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

    DataMemoryFlash flash{ memory };

    if (!copy_sd_to_flash("fk-bundled-fkb.bin", &flash, FK_MEMORY_QSPI_ADDRESS_UPGRADE_CORE, 4096, pool)) {
        logerror("error copying from sd");
        return;
    }

    if (!copy_sd_to_flash("fk-bundled-fkb.bin", &flash, FK_MEMORY_QSPI_ADDRESS_FAILSAFE_CORE, 4096, pool)) {
        logerror("error copying from sd");
        return;
    }

    if (false) {
        if (!copy_memory_to_flash(&flash, build_samd51_modules_dynamic_main_fkdynamic_fkb_bin,
                                build_samd51_modules_dynamic_main_fkdynamic_fkb_bin_len, 512 * 1024,
                                4096, pool)) {
            logerror("error copying binary");
            return;
        }
    }

    FirmwareStorage firmware;
    firmware.walk<bool>([](fkb_header_t const *fkbh) {
        fkb_log_header(fkbh);
        return nullopt;
    });

    if (false) {
        firmware.walk<bool>([&](fkb_header_t const *fkbh) {
            if (fkbh->firmware.flags > 0) {
                fkb_log_header(fkbh);
                fk_module_run(fkbh, pool);
            }
            return nullopt;
        });
    }

    while (true) {
        os_delay(1000);
    }
}

static fkb_symbol_t const *get_symbol_by_index(fkb_header_t const *header, uint32_t symbol) {
    uint8_t const *base = (uint8_t const *)header + header->firmware.tables_offset;
    return (fkb_symbol_t *)(base + sizeof(fkb_symbol_t) * symbol);
}

static fkb_symbol_t const *get_first_symbol(fkb_header_t const *header) {
    return get_symbol_by_index(header, 0);
}

} // namespace fk

#endif
