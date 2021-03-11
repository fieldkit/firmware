#include <tiny_printf.h>
#include <inttypes.h>
#include <phylum/blake2b.h>

#include "backup_worker.h"

#include "hal/hal.h"
#include "hal/memory.h"
#include "hal/sd_card.h"

#include "clock.h"
#include "records.h"

#include "storage/signed_log.h"

namespace fk {

FK_DECLARE_LOGGER("backup");

BackupWorker::BackupWorker() {
}

void BackupWorker::run(Pool &pool) {
    get_board()->i2c_core().begin();

    auto clock = get_clock();
    if (!clock->begin()) {
        logerror("rtc error");
    }

    auto lock = storage_mutex.acquire(UINT32_MAX);

    if (!MemoryFactory::get_data_memory()->begin()) {
        logerror("error starting storage");
        return;
    }

    StatisticsMemory memory{ MemoryFactory::get_data_memory() };

    Storage storage{ &memory, pool };
    if (!storage.begin()) {
        logerror("error opening storage");
        return;
    }

    auto sd = get_sd_card();
    if (!sd->begin()) {
        logerror("error opening sd card");
        return;
    }

    FormattedTime formatted{ get_clock_now(), TimeFormatMachine };
    loginfo("mkdir %s", formatted.cstr());
    if (!sd->mkdir(formatted.cstr())) {
        logerror("error making directory '%s'", formatted.cstr());
        return;
    }

    auto meta_path = pool.sprintf("/%s/meta.fkpb", formatted.cstr());
    auto meta_file = storage.file(Storage::Meta);
    if (!write_file(meta_file, meta_path, pool)) {
        return;
    }

    auto data_path = pool.sprintf("/%s/data.fkpb", formatted.cstr());
    auto data_file = storage.file(Storage::Data);
    if (!write_file(data_file, data_path, pool)) {
        return;
    }
}

bool BackupWorker::write_file(File &file, const char *path, Pool &pool) {
    auto info = file.get_size(0, UINT32_MAX, pool);

    loginfo("total size: %" PRIu32, info.size);

    auto sd = get_sd_card();
    if (!sd->begin()) {
        return false;
    }

    auto writing = sd->open(path, OpenFlags::Write, pool);
    if (writing == nullptr || !*writing) {
        return false;
    }

    BLAKE2b b2b;
    b2b.reset(Hash::Length);

    auto buffer = reinterpret_cast<uint8_t*>(pool.malloc(NetworkBufferSize));
    auto bytes_copied = (uint32_t)0;

    while (bytes_copied < info.size) {
        auto to_read = std::min<int32_t>(NetworkBufferSize, info.size - bytes_copied);
        auto bytes = file.read(buffer, to_read);
        if (bytes > 0) {
            b2b.update(buffer, bytes);
            if (writing->write(buffer, bytes) == bytes) {
                bytes_copied += bytes;
            }
        }
        else {
            break;
        }
    }

    writing->close();

    uint8_t hash[Hash::Length];
    b2b.finalize(&hash, Hash::Length);

    auto hash_hex = bytes_to_hex_string_pool(hash, Hash::Length, pool);

    auto hash_file = sd->open(pool.sprintf("%s.hash", path), OpenFlags::Write, pool);
    if (hash_file == nullptr || !*hash_file) {
        return false;
    }

    hash_file->write((uint8_t *)hash_hex, strlen(hash_hex));

    hash_file->close();

    loginfo("done writing %" PRIu32 " bytes hash=%s", bytes_copied, hash_hex);

    return true;
}

} // namespace fk
