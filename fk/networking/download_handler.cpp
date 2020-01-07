#include "networking/download_handler.h"
#include "utilities.h"
#include "io.h"
#include "hal/hal.h"
#include "progress_tracker.h"
#include "gs_progress_callbacks.h"
#include "storage/storage.h"

namespace fk {

FK_DECLARE_LOGGER("download");

DownloadWorker::DownloadWorker(HttpServerConnection *connection, uint8_t file_number) : connection_(connection), file_number_(file_number) {
}

DownloadWorker::SizeInfo DownloadWorker::get_size(File &file, uint32_t first_block, uint32_t last_block, Pool &pool) {
    if (first_block >= last_block) {
        return {
            .size = 0,
            .last_block = LastRecord,
        };
    }

    if (!file.seek(last_block)) {
        return {
            .size = 0,
            .last_block = LastRecord,
        };
    }

    auto final_position = file.position();
    auto actual_last_block = file.record();

    if (!file.seek(first_block)) {
        return {
            .size = 0,
            .last_block = LastRecord,
        };
    }

    auto start_position = file.position();
    auto size = final_position - start_position;

    return {
        .size = size,
        .last_block = actual_last_block,
    };
}

DownloadWorker::HeaderInfo DownloadWorker::get_headers(File &file, Pool &pool) {
    fk_serial_number_t sn;

    auto gs = get_global_state_ro();

    uint32_t first_block = 0;
    uint32_t last_block = LastRecord;

    auto first_qs = connection_->find_query_param("first", pool);
    if (first_qs != nullptr) {
        first_block = strtol(first_qs, nullptr, 10);
    }

    auto last_qs = connection_->find_query_param("last", pool);
    if (last_qs != nullptr) {
        last_block = strtol(last_qs, nullptr, 10);
    }

    // Calculate the size.
    auto size_info = get_size(file, first_block, last_block, pool);

    loginfo("last_block = #%" PRIu32 " actual_lb = #%" PRIu32 " record = #%" PRIu32, last_block, size_info.last_block, file.record());

    return HeaderInfo{
        .size = size_info.size,
        .first_block = first_block,
        .last_block = size_info.last_block,
        .device_id = bytes_to_hex_string_pool((uint8_t *)&sn, sizeof(sn), pool),
        .generation = bytes_to_hex_string_pool(gs.get()->general.generation, GenerationLength, pool),
    };
}

void DownloadWorker::run(Pool &pool) {
    loginfo("downloading");

    auto lock = storage_mutex.acquire(UINT32_MAX);

    auto started = fk_uptime();
    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory };

    if (!storage.begin()) {
        connection_->error("error opening storage");
        return;
    }

    auto is_head = connection_->is_head_method();
    auto file = storage.file(file_number_);
    auto info = get_headers(file, pool);

    if (info.first_block > info.last_block) {
        logwarn("range #%" PRIu32 " - #%" PRIu32 " size = %" PRIu32 " %s", info.first_block, info.last_block, info.size, is_head ? "HEAD": "GET");
    }
    else {
        loginfo("range #%" PRIu32 " - #%" PRIu32 " size = %" PRIu32 " %s", info.first_block, info.last_block, info.size, is_head ? "HEAD": "GET");
    }

    memory.log_statistics();

    if (!write_headers(info)) {
        connection_->close();
        return;
    }

    if (is_head || info.size == 0) {
        connection_->close();
        return;
    }

    size_t buffer_size = 1024;
    uint8_t *buffer = (uint8_t *)pool.malloc(buffer_size);

    GlobalStateProgressCallbacks gs_progress;
    auto tracker = ProgressTracker{ &gs_progress, Operation::Download, "download", "", info.size };
    auto bytes_copied = 0u;
    while (bytes_copied < info.size) {
        auto to_read = std::min<int32_t>(buffer_size, info.size - bytes_copied);
        auto bytes_read = file.read(buffer, to_read);
        FK_ASSERT(bytes_read == to_read);

        auto wrote = connection_->write(buffer, to_read);
        if (wrote != (int32_t)to_read) {
            logwarn("write error (%" PRId32 " != %" PRId32 ")", wrote, to_read);
            break;
        }

        tracker.update(bytes_read);

        bytes_copied += bytes_read;
    }

    tracker.finished();

    auto elapsed = fk_uptime() - started;
    auto speed = ((bytes_copied / 1024.0f) / (elapsed / 1000.0f));
    loginfo("done (%d) (%" PRIu32 "ms) %.2fkbps", bytes_copied, elapsed, speed);

    connection_->close();

    memory.log_statistics();
}

bool DownloadWorker::write_headers(HeaderInfo header_info) {
    StackBufferedWriter<StackBufferSize> buffered{ connection_ };

    auto status = connection_->is_head_method() ? 204 : 200;

    #define CHECK(expr)  if ((expr) == 0) { return false; }
    CHECK(buffered.write("HTTP/1.1 %d OK\n", status));
    CHECK(buffered.write("Content-Length: %" PRIu32 "\n", header_info.size));
    CHECK(buffered.write("Content-Type: %s\n", "application/octet-stream"));
    CHECK(buffered.write("Connection: close\n"));
    CHECK(buffered.write("Fk-Blocks: %" PRIu32 ", %" PRIu32 "\n", header_info.first_block, header_info.last_block));
    CHECK(buffered.write("Fk-DeviceId: %s\n", header_info.device_id));
    CHECK(buffered.write("Fk-Generation: %s\n\n", header_info.generation));

    return true;
}

DownloadHandler::DownloadHandler(uint8_t file_number) : file_number_(file_number) {
}

bool DownloadHandler::handle(HttpServerConnection *connection, Pool &pool) {
    auto worker = create_pool_worker<DownloadWorker>(connection, file_number_);
    get_ipc()->launch_worker(WorkerCategory::Transfer, worker);
    return true;
}

}
