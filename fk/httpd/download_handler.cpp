#include "httpd/download_handler.h"
#include "storage/storage.h"

namespace fk {

FK_DECLARE_LOGGER("download");

DownloadWorker::DownloadWorker(HttpRequest *req) : req_(req) {
}

void DownloadWorker::run(WorkerContext &wc) {
    loginfo("downloading");

    uint32_t file_number = 0;
    uint32_t first_block = 0;
    uint32_t last_block = LastRecord;

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

    auto info = HeaderInfo{
        .size = size,
        .first_block = first_block,
        .last_block = actual_last_block,
    };
    if (write_headers(info)) {
        // TOOO: MALLOC
        uint8_t *buffer = (uint8_t *)malloc(1024);

        auto bytes_copied = (size_t)0;
        while (bytes_copied < size) {
            auto to_read = std::min<size_t>(sizeof(buffer), size - bytes_copied);
            auto bytes_read = file.read(buffer, to_read);
            FK_ASSERT(bytes_read == to_read);

            if (req_->connection()->write(buffer, to_read) != (int32_t)to_read) {
                logwarn("write error");
                break;
            }

            bytes_copied += bytes_read;
        }

        free(buffer);

        loginfo("done (%d)", bytes_copied);
    }

    req_->connection()->close();
}

bool DownloadWorker::write_headers(HeaderInfo header_info) {
    #define CHECK(expr)  if ((expr) == 0) { return false; }
    CHECK(req_->connection()->write("HTTP/1.1 200 OK\n"));
    CHECK(req_->connection()->write("Content-Length: %" PRIu32 "\n", header_info.size));
    CHECK(req_->connection()->write("Content-Type: %s\n", "application/octet-stream"));
    CHECK(req_->connection()->write("Connection: close\n"));
    CHECK(req_->connection()->write("Fk-Sync: %" PRIu32 ", %" PRIu32 "\n", header_info.first_block, header_info.last_block));
    CHECK(req_->connection()->write("\n"));

    return true;
}

bool DownloadHandler::handle(HttpRequest &req) {
    // TODO: MALLOC
    get_ipc()->launch_worker(new DownloadWorker(&req));

    return true;
}

}
