#include <tiny_printf.h>

#include "dump_flash_memory_worker.h"

#include "hal/hal.h"
#include "hal/memory.h"
#include "utilities.h"
#include "clock.h"
#include "state_manager.h"

namespace fk {

FK_DECLARE_LOGGER("dumpmem");

constexpr size_t MaximumBadBlockRun = 10;

static bool is_phylum() {
    auto gs = get_global_state_ro();
    return gs.get()->storage.is_phylum();
}

void DumpFlashMemoryWorker::run(Pool &pool) {
    auto lock = storage_mutex.acquire(UINT32_MAX);

    FormattedTime formatted{ get_clock_now(), TimeFormatMachine };
    auto path = pool.sprintf("/%s/%08x.bin", formatted.cstr(), 0);

    auto sd = get_sd_card();
    if (!sd->begin()) {
        logerror("error opening sd card");
        return;
    }

    if (!sd->mkdir(formatted.cstr())) {
        logerror("error making directory '%s'", formatted.cstr());
        return;
    }

    auto file = sd->open(path, OpenFlags::Write, pool);
    if (file == nullptr || !file) {
        logerror("unable to open '%s'", path);
        return;
    }

    if (is_phylum()) {
        dump_phylum_storage(file, pool);
    } else {
        dump_darwin_storage(file, pool);
    }

    if (!file->close()) {
        logerror("error closing");
        return;
    }

    GlobalStateManager gsm;
    gsm.notify("flash copied");
}

void DumpFlashMemoryWorker::dump_phylum_storage(SdCardFile *file, Pool &pool) {
    auto flash_memory = MemoryFactory::get_data_memory();
    if (!flash_memory->begin()) {
        logerror("error opening flash memory");
        return;
    }

    auto sector_size = flash_memory->geometry().real_page_size;
    standard_page_buffer_memory buffer_memory{ &pool };
    phylum::working_buffers buffers{ &buffer_memory, sector_size, 8 };
    phylum::noop_page_cache page_cache;
    PhylumFlashMemory memory(flash_memory, &buffers);
    phylum::dhara_sector_map sectors{ buffers, memory, &page_cache };

    if (sectors.begin(false) < 0) {
        logerror("error initializing sector map");
        return;
    }

    auto buffer = (uint8_t *)pool.malloc(sector_size);
    auto nsectors = sectors.size();

    for (auto sector = 0u; sector < nsectors; ++sector) {
        auto err = sectors.read(sector, buffer, sector_size);
        if (err < 0) {
            logerror("error reading sector %d (%d)", sector, err);
            return;
        }

        if (file->write(buffer, sector_size) == 0) {
            logerror("error writing to file");
            return;
        }
    }

    loginfo("done writing %d sectors (%d bytes)", nsectors, nsectors * sector_size);
}

void DumpFlashMemoryWorker::dump_darwin_storage(SdCardFile *file, Pool &pool) {
    auto flash_memory = MemoryFactory::get_data_memory();
    if (!flash_memory->begin()) {
        logerror("error opening flash memory");
        return;
    }

    auto g = flash_memory->geometry();
    auto memory = SequentialMemory{ flash_memory };
    auto buffer = (uint8_t *)pool.malloc(g.page_size);

    auto version = (uint32_t)0u;
    auto bad_blocks = 0u;

    for (auto address = g.beginning(); address < g.end();) {
        auto file_size = file->file_size();

        if (bad_blocks == MaximumBadBlockRun) {
            break;
        }

        if (memory.read(address, buffer, g.page_size) != (int32_t)g.page_size) {
            logerror("error reading memory");
            return;
        }

        if ((address % g.block_size) == 0) {
            loginfo("" PRADDRESS " copying (%zd bytes)", address, file_size);

            BlockHeader block_header;
            memcpy(&block_header, buffer, sizeof(block_header));

            if (block_header.verify_hash()) {
                bad_blocks = 0;

                if (version == 0) {
                    version = block_header.version;
                    loginfo("" PRADDRESS " found version (%" PRIx32 ")", address, version);
                } else if (version != block_header.version) {
                    loginfo("" PRADDRESS " wrong version (%" PRIx32 " %" PRIx32 ")", address, version, block_header.version);
                    break;
                }
            } else {
                if (is_memory_erased(buffer, g.page_size)) {
                    loginfo("" PRADDRESS " erased block", address);
                } else if (is_memory_zeros(buffer, g.page_size)) {
                    loginfo("" PRADDRESS " bad block", address);
                } else {
                    loginfo("" PRADDRESS " invalid block header", address);
                }
                bad_blocks++;
            }
        }

        if (file->write(buffer, g.page_size) == 0) {
            logerror("error writing to file");
            return;
        }

        address += g.page_size;
    }
}

} // namespace fk
