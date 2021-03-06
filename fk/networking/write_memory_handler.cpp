#include <samd51_common.h>

#include "networking/write_memory_handler.h"
#include "gs_progress_callbacks.h"
#include "progress_tracker.h"
#include "hal/flash.h"

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

    auto buffer = reinterpret_cast<uint8_t*>(pool.malloc(NetworkBufferSize));
    auto bytes_copied = (uint32_t)0;

    while (connection_->active() && bytes_copied < expected) {
        auto bytes = connection_->read(buffer, NetworkBufferSize);
        if (bytes > 0) {
            if (writer.write(buffer, bytes) == bytes) {
                bytes_copied += bytes;
            }

            tracker.update(bytes);
        }
    }

    tracker.finished();

    if (bytes_copied != expected) {
        logwarn("unexpected bytes %" PRIu32 " != %" PRIu32, bytes_copied, expected);
        write_error("incomplete", pool);
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
