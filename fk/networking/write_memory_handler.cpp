#include <samd51_common.h>

#include "networking/write_memory_handler.h"
#include "gs_progress_callbacks.h"
#include "progress_tracker.h"
#include "storage/types.h"
#include "hal/flash.h"
#include "sd_copying.h"

namespace fk {

FK_DECLARE_LOGGER("writemem");

WriteMemoryWorker::WriteMemoryWorker(HttpServerConnection *connection) : connection_(connection) {
}

bool WriteMemoryWorker::read_complete_and_fail(const char *error, Pool &pool) {
    auto expected = connection_->length();
    auto bytes_copied = (uint32_t)0;
    auto buffer = reinterpret_cast<uint8_t*>(pool.malloc(NetworkBufferSize));

    while (connection_->active() && bytes_copied < expected) {
        auto bytes = connection_->read(buffer, NetworkBufferSize);
        if (bytes > 0) {
            bytes_copied += bytes;
        }
    }

    return write_error(error, pool);
}

bool WriteMemoryWorker::write_error(const char *kind, Pool &pool) {
    auto body = pool.sprintf("{ \"%s\": true }", kind);
    connection_->plain(500, "error", body);
    connection_->close();
    return true;
}

bool WriteMemoryWorker::write_success(Pool &pool) {
    auto body = pool.sprintf("{ \"%s\": true }", "success");
    connection_->plain(200, "ok", body);
    connection_->close();
    return true;
}

void WriteMemoryWorker::run(Pool &pool) {
    auto lock = sd_mutex.acquire(UINT32_MAX);
    auto expected = connection_->length();

    GlobalStateProgressCallbacks gs_progress;
    ProgressTracker tracker{ &gs_progress, Operation::Download, "receiving", "", expected };

    auto address_str = connection_->find_query_param("address", pool);
    if (address_str == nullptr) {
        logwarn("missing address");
        read_complete_and_fail("address", pool);
        return;
    }

    uint32_t address = atoi(address_str);
    if (address < 0 || address > 8 * 1024 * 1024) {
        logwarn("invalid address");
        read_complete_and_fail("address", pool);
        return;
    }

    loginfo("receiving %" PRIu32 " bytes, writing to 0x%08" PRIx32 "", expected, address);

    auto memory = MemoryFactory::get_qspi_memory();
    DataMemoryFlash flash{ memory };
    FlashWriter writer{ &flash, address };
    BufferedWriter buffered{ &writer, (uint8_t *)pool.malloc(NetworkBufferSize), NetworkBufferSize };

    auto buffer = reinterpret_cast<uint8_t*>(pool.malloc(NetworkBufferSize));
    auto bytes_copied = (uint32_t)0;

    while (connection_->active() && bytes_copied < expected) {
        auto nread = std::min<size_t>(NetworkBufferSize, expected - bytes_copied);
        auto bytes = connection_->read(buffer, nread);
        if (bytes > 0) {
            if (buffered.write(buffer, bytes) == bytes) {
                bytes_copied += bytes;
            }

            tracker.update(bytes);
        }
    }

    buffered.flush();

    tracker.finished();

    if (bytes_copied != expected) {
        logwarn("unexpected bytes %" PRIu32 " != %" PRIu32, bytes_copied, expected);
        write_error("incomplete", pool);
        // TODO Nuke first page?
        return;
    }

    Hash expected_hash;
    if (!flash.read((address + bytes_copied) - Hash::Length, (uint8_t *)&expected_hash.hash, Hash::Length)) {
        logerror("error reading hash");
        write_error("hash_read", pool);
        // TODO Nuke first page?
        return;
    }

    auto hash_check = verify_flash_binary_hash(&flash, address, bytes_copied, 4096, expected_hash, pool);
    if (!hash_check) {
        logwarn("error checking hash");
        write_error("hash_check", pool);
        // TODO Nuke first page?
        return;
    }

    if (!*hash_check) {
        logwarn("hash mismatch");
        write_error("hash_mismatch", pool);
        // TODO Nuke first page?
        return;
    }

    write_success(pool);
}

bool WriteMemoryHandler::handle(HttpServerConnection *connection, Pool &pool) {
    auto worker = create_pool_worker<WriteMemoryWorker>(connection);
    get_ipc()->launch_worker(WorkerCategory::Transfer, worker);
    return true;
}

}
