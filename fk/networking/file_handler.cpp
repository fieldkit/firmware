#include "networking/file_handler.h"
#include "utilities.h"
#include "io.h"
#include "hal/hal.h"
#include "progress_tracker.h"
#include "gs_progress_callbacks.h"
#include "storage/storage.h"

namespace fk {

FK_DECLARE_LOGGER("sendfile");

DownloadFileWorker::DownloadFileWorker(HttpServerConnection *connection) : connection_(connection) {
}

DownloadFileWorker::HeaderInfo DownloadFileWorker::get_headers(SdCardFile *file, Pool &pool) {
    fk_serial_number_t sn;
    auto gs = get_global_state_ro();
    return HeaderInfo{
        .size = (uint32_t)file->file_size(),
        .device_id = bytes_to_hex_string_pool((uint8_t *)&sn, sizeof(sn), pool),
        .generation = bytes_to_hex_string_pool(gs.get()->general.generation, GenerationLength, pool),
    };
}

void DownloadFileWorker::run(Pool &pool) {
    serve(pool);
    connection_->busy(false);
}

void DownloadFileWorker::serve(Pool &pool) {
    auto lock = sd_mutex.acquire(UINT32_MAX);

    auto started = fk_uptime();
    auto is_head = connection_->is_head_method();
    auto path = connection_->find_query_param("p", pool);
    if (path == nullptr) {
        connection_->plain(HttpStatus::BadRequest, "path required", "path required", pool);
        return;
    }

    loginfo("downloading file: %s", path);

    auto sd = get_sd_card();
    auto file = sd->open(path, OpenFlags::Read, pool);
    if (file == nullptr || !file->is_open()) {
        connection_->plain(HttpStatus::NotFound, "not found", "not found", pool);
        return;
    }

    auto info = get_headers(file, pool);

    loginfo("downloading file size=%" PRIu32, info.size);

    if (!write_headers(info)) {
        connection_->close();
        return;
    }

    if (is_head || info.size == 0) {
        connection_->close();
        return;
    }

    GlobalStateProgressCallbacks gs_progress;
    ProgressTracker tracker{ &gs_progress, Operation::Download, "sendfile", "", info.size };
    BufferedWriter writer{ connection_, (uint8_t *)pool.malloc(NetworkBufferSize), NetworkBufferSize };

    auto buffer = reinterpret_cast<uint8_t *>(pool.malloc(NetworkBufferSize));
    auto bytes_copied = 0u;

    while (connection_->active() && bytes_copied < info.size) {
        auto bytes = file->read(buffer, NetworkBufferSize);
        if (bytes > 0) {
            if (connection_->write(buffer, bytes) == bytes) {
                bytes_copied += bytes;
            }

            tracker.update(bytes);
        }
    }

    writer.flush();

    tracker.finished();

    auto elapsed = fk_uptime() - started;
    auto speed = ((bytes_copied / 1024.0f) / (elapsed / 1000.0f));
    loginfo("done (%d) (%" PRIu32 "ms) %.2fkbps", bytes_copied, elapsed, speed);

    connection_->close();
}

bool DownloadFileWorker::write_headers(HeaderInfo header_info) {
    StackBufferedWriter<StackBufferSize> buffered{ connection_ };

#define CHECK(expr)                                                                                                                        \
    if ((expr) == 0) {                                                                                                                     \
        return false;                                                                                                                      \
    }
    CHECK(buffered.write("HTTP/1.1 %d OK\n", 200));
    CHECK(buffered.write("Content-Length: %" PRIu32 "\n", header_info.size));
    CHECK(buffered.write("Content-Type: %s\n", "text/plain"));
    CHECK(buffered.write("Connection: close\n"));
    CHECK(buffered.write("Fk-DeviceId: %s\n", header_info.device_id));
    CHECK(buffered.write("Fk-Generation: %s\n\n", header_info.generation));

    return true;
}

DownloadFileHandler::DownloadFileHandler() {
}

bool DownloadFileHandler::handle(HttpServerConnection *connection, Pool &pool) {
    // The two calls are annoying, necessary to avoid races.
    connection->busy(true);
    auto worker = create_pool_worker<DownloadFileWorker>(connection);
    if (!get_ipc()->launch_worker(WorkerCategory::Transfer, worker)) {
        connection->busy(false);
    }
    return true;
}

} // namespace fk
