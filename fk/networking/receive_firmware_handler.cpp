#include "networking/receive_firmware_handler.h"

#include "gs_progress_callbacks.h"

namespace fk {

FK_DECLARE_LOGGER("receive");

ReceiveFirmwareWorker::ReceiveFirmwareWorker(HttpServerConnection *connection) : connection_(connection) {
}

void ReceiveFirmwareWorker::run(Pool &pool) {
    auto lock = sd_mutex.acquire(UINT32_MAX);
    auto expected = connection_->length();
    auto bytes_copied = (uint32_t)0;

    loginfo("receiving %" PRIu32 " bytes...", expected);

    auto buffer = (uint8_t *)pool.malloc(1024);
    auto success = true;

    while (connection_->active() && bytes_copied < expected) {
        auto bytes = connection_->read(buffer, 1024);
        if (bytes > 0) {
            bytes_copied += bytes;
        }
    }

    logwarn("done (%" PRIu32 " bytes)", bytes_copied);

    if (bytes_copied != expected) {
        success = false;
    }

    if (success) {
        connection_->plain(200, "ok", "{}");
    }
    else {
        connection_->plain(500, "error", "{}");
    }

    connection_->close();
}

bool ReceiveFirmwareHandler::handle(HttpServerConnection *connection, Pool &pool) {
    auto worker = create_pool_worker<ReceiveFirmwareWorker>(connection);
    get_ipc()->launch_worker(WorkerCategory::Transfer, worker);
    return true;
}

}
