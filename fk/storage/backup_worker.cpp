#include <tiny_printf.h>
#include <inttypes.h>
#include <blake2b.h>

#include "backup_worker.h"

#include "hal/hal.h"
#include "hal/memory.h"
#include "hal/sd_card.h"

#include "clock.h"
#include "records.h"

#include "storage/signed_log.h"

namespace fk {

FK_DECLARE_LOGGER("backup");

BackupWorker::BackupWorker() : info_{ "Backup", 0.0f, true } {
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

    if (!storage.is_phylum()) {
        auto meta_path = pool.sprintf("/%s/meta.fkpb", formatted.cstr());
        auto meta_file = storage.file_reader(Storage::Meta, pool);
        if (!write_file(meta_file, meta_path, pool)) {
            return;
        }
    }

    auto data_path = pool.sprintf("/%s/data.fkpb", formatted.cstr());
    auto data_file = storage.file_reader(Storage::Data, pool);
    if (!write_file(data_file, data_path, pool)) {
        return;
    }
}

bool BackupWorker::write_file(FileReader *file, const char *path, Pool &pool) {
    auto info = file->get_size(0, UINT32_MAX, pool);
    if (!info) {
        logerror("get-size");
        return false;
    }

    loginfo("total size: %" PRIu32, info->size);

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

    auto buffer = reinterpret_cast<uint8_t *>(pool.malloc(NetworkBufferSize));
    auto bytes_copied = (uint32_t)0;

    ScopedLogLevelChange log_level_info_only{ LogLevels::INFO };

    while (bytes_copied < info->size) {
        auto to_read = std::min<int32_t>(NetworkBufferSize, info->size - bytes_copied);
        auto bytes = file->read(buffer, to_read);
        if (bytes > 0) {
            b2b.update(buffer, bytes);
            if (writing->write(buffer, bytes) == bytes) {
                bytes_copied += bytes;
            }

            info_.progress = (float)bytes_copied / info->size;
        } else {
            break;
        }
    }

    writing->close();

    uint8_t expected[Hash::Length];
    b2b.finalize(&expected, Hash::Length);

    auto expected_hex = bytes_to_hex_string_pool(expected, Hash::Length, pool);

    auto hash_writer = sd->open(pool.sprintf("%s.hash", path), OpenFlags::Write, pool);
    if (hash_writer == nullptr || !*hash_writer) {
        return false;
    }

    hash_writer->write((uint8_t *)expected_hex, strlen(expected_hex));

    hash_writer->close();

    loginfo("done writing %" PRIu32 " bytes hash=%s", bytes_copied, expected_hex);

    if (bytes_copied == 0) {
        logwarn("empty file '%s'", path);
        return true;
    }

    if (verify_) {
        uint8_t actual[Hash::Length];
        if (!hash_file(path, actual, pool)) {
            return false;
        }

        if (memcmp(actual, expected, Hash::Length) != 0) {
            logerror("hash mismatch!");
            fk_dump_memory("expected ", (uint8_t *)&expected, Hash::Length);
            fk_dump_memory("actual   ", (uint8_t *)&actual, Hash::Length);
        }
    }

    return true;
}

bool BackupWorker::hash_file(const char *path, uint8_t *hash, Pool &pool) {
    auto sd = get_sd_card();

    auto file = sd->open(path, OpenFlags::Read, pool);
    if (file == nullptr || !file->is_open()) {
        logerror("unable to open '%s'", path);
        return false;
    }

    auto file_size = file->file_size();
    if (file_size == 0) {
        logwarn("empty file '%s'", path);
        return true;
    }

    file->seek_beginning();

    BLAKE2b b2b;
    b2b.reset(Hash::Length);

    auto total_bytes = (uint32_t)0u;
    auto buffer = (uint8_t *)pool.malloc(NetworkBufferSize);
    while (total_bytes < file_size) {
        auto nread = file->read(buffer, NetworkBufferSize);
        if (nread <= 0) {
            break;
        }

        b2b.update(buffer, nread);

        total_bytes += nread;
    }

    b2b.finalize(hash, Hash::Length);

    return true;
}

} // namespace fk
