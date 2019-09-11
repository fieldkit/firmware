#include "networking/receive_handler.h"
#include "storage/storage.h"

namespace fk {

FK_DECLARE_LOGGER("receive");

ReceiveWorker::ReceiveWorker(Connection *connection) : connection_(connection) {
}

void ReceiveWorker::run(Pool &pool) {
    auto expected = connection_->length();
    auto bytes_copied = (uint32_t)0;

    loginfo("receiving %" PRIu32 " bytes...", expected);

    while (bytes_copied < expected) {
        uint8_t buffer[1024];
        auto bytes = connection_->read(buffer, sizeof(buffer));
        if (bytes == 0) {
            logwarn("unexpected eof");
            break;
        }

        bytes_copied += bytes;
    }

    logwarn("done (%" PRIu32 " bytes)", bytes_copied);

    connection_->close();
}

bool ReceiveHandler::handle(Connection *connection, Pool &pool) {
    auto worker = create_pool_wrapper<ReceiveWorker, DefaultWorkerPoolSize, PoolWorker<ReceiveWorker>>(connection);
    if (!get_ipc()->launch_worker(WorkerCategory::Transfer, worker)) {
        delete worker;
        return false;
    }
    return true;
}

}
