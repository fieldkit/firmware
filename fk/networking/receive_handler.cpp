#include "networking/receive_handler.h"
#include "storage/storage.h"

namespace fk {

FK_DECLARE_LOGGER("receive");

ReceiveWorker::ReceiveWorker(HttpRequest *req) : req_(req) {
    auto expected = req_->length();
    auto bytes_copied = (uint32_t)0;

    loginfo("receiving %d bytes...", expected);

    while (bytes_copied < expected) {
        uint8_t buffer[1024];
        auto bytes = req_->connection()->read(buffer, sizeof(buffer));
        if (bytes == 0) {
            logwarn("unexpected eof");
            break;
        }

        bytes_copied += bytes;
    }

    logwarn("done (%d bytes)", bytes_copied);

    req_->connection()->close();
}

void ReceiveWorker::run(WorkerContext &wc) {
}

bool ReceiveHandler::handle(HttpRequest &req) {
    // TODO: MALLOC
    if (!get_ipc()->launch_worker(new ReceiveWorker(&req))) {
        return false;
    }

    return true;
}

}
