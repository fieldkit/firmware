#include <samd51_common.h>

#include "networking/receive_firmware_handler.h"
#include "upgrade_from_sd_worker.h"
#include "gs_progress_callbacks.h"
#include "graceful_shutdown.h"

namespace fk {

FK_DECLARE_LOGGER("receive");

ReceiveFirmwareWorker::ReceiveFirmwareWorker(HttpServerConnection *connection) : connection_(connection) {
}

bool ReceiveFirmwareWorker::read_complete_and_fail(const char *error, Pool &pool) {
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

bool ReceiveFirmwareWorker::write_error(const char *kind, Pool &pool) {
    auto body = pool.sprintf("{ \"%s\": true }", kind);
    connection_->plain(500, "error", body);
    connection_->close();
    return true;
}

bool ReceiveFirmwareWorker::write_success(Pool &pool) {
    auto body = pool.sprintf("{ \"%s\": true }", "success");
    connection_->plain(200, "ok", body);
    connection_->close();
    return true;
}

void ReceiveFirmwareWorker::run(Pool &pool) {
    auto lock = sd_mutex.acquire(UINT32_MAX);
    auto file_name = "fk-bundled-fkb-network.bin";
    auto expected = connection_->length();

    loginfo("receiving %" PRIu32 " bytes...", expected);

    GlobalStateProgressCallbacks gs_progress;
    ProgressTracker tracker{ &gs_progress, Operation::Download, "receiving", "", expected };

    auto swap = connection_->find_query_param("swap", pool) != nullptr;
    auto sd = get_sd_card();
    if (!sd->begin()) {
        read_complete_and_fail("sdCard", pool);
        return;
    }

    if (sd->is_file(file_name)) {
        loginfo("deleting existing file");
        if (!sd->unlink(file_name)) {
            read_complete_and_fail("unlink", pool);
            return;
        }
    }

    auto file = sd->open(file_name, OpenFlags::Write, pool);
    if (file == nullptr || !*file) {
        read_complete_and_fail("create", pool);
    }

    auto buffer = reinterpret_cast<uint8_t*>(pool.malloc(NetworkBufferSize));
    auto bytes_copied = (uint32_t)0;

    while (connection_->active() && bytes_copied < expected) {
        auto bytes = connection_->read(buffer, NetworkBufferSize);
        if (bytes > 0) {
            if (file->write(buffer, bytes) == bytes) {
                bytes_copied += bytes;
            }

            tracker.update(bytes);
        }
    }

    file->close();

    tracker.finished();

    if (bytes_copied != expected) {
        write_error("incomplete", pool);
        return;
    }

    write_success(pool);

    if (!swap) {
        loginfo("just writing to card");
        return;
    }

    auto marker_file_name = "upgrade.cfg";
    auto marker_file = sd->open(marker_file_name, OpenFlags::Write, pool);
    if (marker_file == nullptr || !marker_file) {
        logerror("error touching %s", marker_file_name);
        return;
    }

    file->close();

    loginfo("graceful shutdown");

    fk_graceful_shutdown();

    loginfo("restarting");

    fk_logs_flush();

    fk_restart();
}

bool ReceiveFirmwareHandler::handle(HttpServerConnection *connection, Pool &pool) {
    auto worker = create_pool_worker<ReceiveFirmwareWorker>(connection);
    get_ipc()->launch_worker(WorkerCategory::Transfer, worker);
    return true;
}

}
