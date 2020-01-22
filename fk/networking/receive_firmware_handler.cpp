#include "networking/receive_firmware_handler.h"
#include "upgrade_from_sd_worker.h"
#include "gs_progress_callbacks.h"

namespace fk {

FK_DECLARE_LOGGER("receive");

ReceiveFirmwareWorker::ReceiveFirmwareWorker(HttpServerConnection *connection) : connection_(connection) {
}

void ReceiveFirmwareWorker::run(Pool &pool) {
    auto lock = sd_mutex.acquire(UINT32_MAX);
    auto expected = connection_->length();
    auto bytes_copied = (uint32_t)0;
    auto file_name = "fk-bundled-fkb-network.bin";

    loginfo("receiving %" PRIu32 " bytes...", expected);

    auto sd = get_sd_card();
    if (sd->is_file(file_name)) {
        if (!sd->unlink(file_name)) {
            connection_->plain(500, "error", "{}");
            connection_->close();
            return;
        }
    }

    auto file = sd->open(file_name, true, pool);
    auto buffer = reinterpret_cast<uint8_t*>(pool.malloc(1024));

    while (connection_->active() && bytes_copied < expected) {
        auto bytes = connection_->read(buffer, 1024);
        if (bytes > 0) {
            if (file->write(buffer, bytes) == bytes) {
                bytes_copied += bytes;
            }
        }
    }

    file->close();

    logwarn("done (%" PRIu32 " bytes)", bytes_copied);

    if (bytes_copied != expected) {
        connection_->plain(500, "error", "{}");
        connection_->close();
        return;
    }

    connection_->plain(200, "ok", "{}");
    connection_->close();

    fk_delay(1000);

    auto params = SdCardFirmware{ SdCardFirmwareOperation::Load, nullptr, file_name, true };
    get_ipc()->launch_worker(create_pool_worker<UpgradeFirmwareFromSdWorker>(params));
}

bool ReceiveFirmwareHandler::handle(HttpServerConnection *connection, Pool &pool) {
    auto worker = create_pool_worker<ReceiveFirmwareWorker>(connection);
    get_ipc()->launch_worker(WorkerCategory::Transfer, worker);
    return true;
}

}
