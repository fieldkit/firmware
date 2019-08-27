#include "networking/receive_handler.h"
#include "storage/storage.h"

namespace fk {

FK_DECLARE_LOGGER("receive");

ReceiveWorker::ReceiveWorker(HttpRequest *req, Pool *pool) : Worker(pool), req_(req) {
    auto expected = req_->length();
    auto bytes_copied = (uint32_t)0;
    auto reader = req->reader();

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

void ReceiveWorker::run(WorkerContext &wc, Pool &pool) {
}

bool ReceiveHandler::handle(HttpRequest &req, Pool &pool) {
    // TODO: MALLOC
    if (!get_ipc()->launch_worker(new ReceiveWorker(&req, nullptr))) {
        return false;
    }

    return true;
}

}
