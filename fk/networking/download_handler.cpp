#include "networking/download_handler.h"
#include "storage/storage.h"
#include "storage/progress.h"
#include "writer.h"
#include "hal/hal.h"

namespace fk {

FK_DECLARE_LOGGER("download");

DownloadWorker::DownloadWorker(HttpRequest *req, uint8_t file_number) : req_(req), file_number_(file_number) {
}

void DownloadWorker::run(WorkerContext &wc) {
    loginfo("downloading");

    auto lock = storage_mutex.acquire(UINT32_MAX);
    auto memory_bus = get_board()->spi_flash();

    uint32_t first_block = 0;
    uint32_t last_block = LastRecord;

    auto started = fk_uptime();
    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory };

    FK_ASSERT(storage.begin());

    auto file = storage.file(file_number_);

    FK_ASSERT(file.seek(last_block));
    auto final_position = file.position();
    // When we seek to LastRecord this is the record that will be written,
    // otherwise it's the record we're on.
    auto actual_last_block = last_block == LastRecord ? file.record() - 1 : file.record();

    FK_ASSERT(file.seek(first_block));
    auto start_position = file.position();

    auto size = final_position - start_position;

    memory.log_statistics();

    auto info = HeaderInfo{
        .size = size,
        .first_block = first_block,
        .last_block = actual_last_block,
    };
    if (write_headers(info)) {
        // TOOO: MALLOC
        size_t buffer_size = 1024;
        uint8_t *buffer = (uint8_t *)malloc(buffer_size);

        auto tracker = ProgressTracker{ "download", "", size };
        auto bytes_copied = (size_t)0;
        while (bytes_copied < size) {
            auto to_read = std::min<size_t>(buffer_size, size - bytes_copied);
            auto bytes_read = file.read(buffer, to_read);
            FK_ASSERT(bytes_read == to_read);

            if (req_->connection()->write(buffer, to_read) != (int32_t)to_read) {
                logwarn("write error");
                break;
            }

            tracker.update(bytes_read);

            bytes_copied += bytes_read;
        }

        free(buffer);

        auto elapsed = fk_uptime() - started;
        auto speed = ((bytes_copied / 1024.0f) / (elapsed / 1000.0f));
        loginfo("done (%zd) (%" PRIu32 "ms) %.2fkbps", bytes_copied, elapsed, speed);
    }

    req_->connection()->close();

    memory.log_statistics();
}

bool DownloadWorker::write_headers(HeaderInfo header_info) {
    BufferedWriter buffered{ req_->connection() };

    #define CHECK(expr)  if ((expr) == 0) { return false; }
    CHECK(buffered.write("HTTP/1.1 200 OK\n"));
    CHECK(buffered.write("Content-Length: %" PRIu32 "\n", header_info.size));
    CHECK(buffered.write("Content-Type: %s\n", "application/octet-stream"));
    CHECK(buffered.write("Connection: close\n"));
    CHECK(buffered.write("Fk-Sync: %" PRIu32 ", %" PRIu32 "\n\n", header_info.first_block, header_info.last_block));

    return true;
}

DownloadHandler::DownloadHandler(uint8_t file_number) : file_number_(file_number) {
}

bool DownloadHandler::handle(HttpRequest &req) {
    // TODO: MALLOC
    if (!get_ipc()->launch_worker(new DownloadWorker(&req, file_number_))) {
        return false;
    }

    return true;
}

}
