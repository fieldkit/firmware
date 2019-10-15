#include <tiny_printf.h>

#include "dump_flash_memory.h"

#include "hal/hal.h"
#include "hal/memory.h"
#include "hal/sd_card.h"
#include "utilities.h"

namespace fk {

FK_DECLARE_LOGGER("dumpmem");

constexpr size_t MaximumBadBlockRun = 10;

void DumpFlashMemory::run(Pool &pool) {
    auto lock = storage_mutex.acquire(UINT32_MAX);
    auto memory = SequentialMemory{ MemoryFactory::get_data_memory() };
    auto g = MemoryFactory::get_data_memory()->geometry();

    auto sd = get_sd_card();
    auto buffer = (uint8_t *)pool.malloc(g.page_size);

    char file_name[13];
    tiny_snprintf(file_name, sizeof(file_name), "%08x.bin", 0);

    auto file = sd->open(file_name, pool);
    if (!file) {
        logerror("unable to open %s", file_name);
        return;
    }

    auto version = (uint32_t)0u;
    auto bad_blocks = 0u;

    for (auto address = g.beginning(); address < g.end(); address += g.page_size) {
        auto file_size = file->file_size();

        if (memory.read(address, buffer, g.page_size) != g.page_size) {
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
                }
                else if (version != block_header.version) {
                    loginfo("" PRADDRESS " wrong version (%" PRIx32 " %" PRIx32 ")", address, version, block_header.version);
                    break;
                }
            }
            else {
                loginfo("" PRADDRESS " invalid block header", address);
                bad_blocks++;
            }

            fk_dump_memory("HDR", (uint8_t *)&block_header, sizeof(block_header));

            if (bad_blocks == MaximumBadBlockRun) {
                break;
            }
        }

        if (file->write(buffer, g.page_size) == 0) {
            logerror("error writing to file");
            return;
        }
    }

    file->close();
}

}