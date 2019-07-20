#include "httpd/download_handler.h"

#include "storage.h"

namespace fk {

FK_DECLARE_LOGGER("download");

bool DownloadHandler::handle(HttpRequest &req) {
    auto query = req.query();
    if (query != nullptr) {
        switch (query->type) {
        default: {
            break;
        }
        }
    }

    // TODO: MALLOC
    get_ipc()->launch_worker(new DownloadWorker(&req));

    return true;
}

DownloadWorker::DownloadWorker(HttpRequest *req) : req_(req) {
}

void DownloadWorker::run(WorkerContext &wc) {
    loginfo("downloading");

    uint32_t file_number = 0;
    uint32_t first_block = 0;
    uint32_t last_block = LastRecord;

    // Required: File, Block

    auto memory = MemoryFactory::get_data_memory();
    Storage storage{ memory };

    auto file = storage.file(file_number);

    FK_ASSERT(file.seek(last_block));
    auto final_position = file.position();
    // When we seek to LastRecord this is the record that will be written,
    // otherwise it's the record we're on.
    auto actual_last_block = last_block == LastRecord ? file.record() - 1 : file.record();

    FK_ASSERT(file.seek(first_block));
    auto start_position = file.position();

    auto size = final_position - start_position;

    loginfo("File: size = %" PRIu32 " record = %" PRIu32 "", file.size(), file.record());
    loginfo("File: start = %" PRIu32 " final = %" PRIu32 "", start_position, final_position);

    req_->connection()->write("HTTP/1.1 200 OK\n");
    req_->connection()->write("Content-Length: %" PRIu32 "\n", size);
    req_->connection()->write("Content-Type: %s\n", "application/octet-stream");
    req_->connection()->write("Connection: close\n");
    req_->connection()->write("Fk-Sync: %" PRIu32 ", %" PRIu32 "\n", first_block, actual_last_block);
    req_->connection()->write("\n");

    auto bytes_copied = (size_t)0;
    while (bytes_copied < size) {
        uint8_t buffer[1024];

        auto to_read = std::min<size_t>(sizeof(buffer), size - bytes_copied);
        auto bytes_read = file.read(buffer, to_read);
        FK_ASSERT(bytes_read == to_read);

        if (req_->connection()->write(buffer, to_read) != (int32_t)to_read) {
            logwarn("write error");
            break;
        }

        bytes_copied += bytes_read;
    }

    loginfo("done (%d)", bytes_copied);

    req_->connection()->close();
}

}
