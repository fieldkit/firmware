#include "networking/logs_handler.h"
#include "utilities.h"
#include "io.h"
#include "hal/hal.h"
#include "progress_tracker.h"
#include "gs_progress_callbacks.h"
#include "storage/storage.h"

namespace fk {

FK_DECLARE_LOGGER("sendlogs");

DownloadLogsWorker::DownloadLogsWorker(HttpServerConnection *connection) : connection_(connection) {
}

DownloadLogsWorker::HeaderInfo DownloadLogsWorker::get_headers(Pool &pool) {
    fk_serial_number_t sn;
    auto size = 0u;

    auto &lb = fk_log_buffer();
    for (auto c : lb) {
        if (c != 0) {
            size++;
        }
    }

    auto gs = get_global_state_ro();
    return HeaderInfo{
        .size = size,
        .device_id = bytes_to_hex_string_pool((uint8_t *)&sn, sizeof(sn), pool),
        .generation = bytes_to_hex_string_pool(gs.get()->general.generation, GenerationLength, pool),
    };
}

void DownloadLogsWorker::run(Pool &pool) {
    serve(pool);
    connection_->busy(false);
}

void DownloadLogsWorker::serve(Pool &pool) {
    loginfo("downloading logs");

    LogBufferLock lock;

    FK_ASSERT(lock);

    auto started = fk_uptime();
    auto is_head = connection_->is_head_method();
    auto info = get_headers(pool);

    if (!write_headers(info)) {
        connection_->close();
        return;
    }

    if (is_head || info.size == 0) {
        connection_->close();
        return;
    }

    GlobalStateProgressCallbacks gs_progress;
    ProgressTracker tracker{ &gs_progress, Operation::Download, "sendlogs", "", info.size };
    BufferedWriter writer{ connection_, (uint8_t *)pool.malloc(NetworkBufferSize), NetworkBufferSize };
    auto bytes_copied = 0u;

    auto &lb = fk_log_buffer();
    for (auto c : lb) {
        if (bytes_copied >= info.size) {
            break;
        }
        if (c != 0) {
            writer.write(c);
            bytes_copied++;
        }
    }

    writer.flush();

    tracker.finished();

    auto elapsed = fk_uptime() - started;
    auto speed = ((bytes_copied / 1024.0f) / (elapsed / 1000.0f));
    loginfo("done (%d) (%" PRIu32 "ms) %.2fkbps", bytes_copied, elapsed, speed);

    connection_->close();
}

bool DownloadLogsWorker::write_headers(HeaderInfo header_info) {
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

DownloadLogsHandler::DownloadLogsHandler() {
}

bool DownloadLogsHandler::handle(HttpServerConnection *connection, Pool &pool) {
    // The two calls are annoying, necessary to avoid races.
    connection->busy(true);
    auto worker = create_pool_worker<DownloadLogsWorker>(connection);
    if (!get_ipc()->launch_worker(WorkerCategory::Transfer, worker)) {
        connection->busy(false);
    }
    return true;
}

} // namespace fk
