#include "networking/receive_handler.h"
#include "storage/storage.h"

namespace fk {

FK_DECLARE_LOGGER("receive");

ReceiveWorker::ReceiveWorker(HttpRequest &req, uint32_t a) : req_(&req) {
}

void ReceiveWorker::run(Pool &pool) {
    auto expected = req_->length();
    auto bytes_copied = (uint32_t)0;
    auto reader = req_->reader();

    loginfo("receiving %" PRIu32 " bytes...", expected);

    while (bytes_copied < expected) {
        uint8_t buffer[1024];
        auto bytes = reader->read(buffer, sizeof(buffer));
        if (bytes == 0) {
            logwarn("unexpected eof");
            break;
        }

        bytes_copied += bytes;
    }

    logwarn("done (%" PRIu32 " bytes)", bytes_copied);

    req_->connection()->close();
}

bool ReceiveHandler::handle(HttpRequest &req, Pool &pool) {
    auto worker = create_pool_worker<ReceiveWorker>(DefaultWorkerPoolSize, req, 0);
    if (!get_ipc()->launch_worker(worker)) {
        return false;
    }

    return true;
}

}
