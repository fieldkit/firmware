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

tl::expected<DownloadWorker::HeaderInfo, Error> DownloadWorker::get_headers(FileReader *file_reader, Pool &pool) {
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
    auto size_info = file_reader->get_size(first_block, last_block, pool);
    if (!size_info) {
        return tl::unexpected<Error>(Error::IO);
    }

    loginfo("last_block = #%" PRIu32 " actual_lb = #%" PRIu32 "", last_block, size_info->last_block);

    return HeaderInfo{
        .size = size_info->size,
        .first_block = first_block,
        .last_block = size_info->last_block,
        .device_id = bytes_to_hex_string_pool((uint8_t *)&sn, sizeof(sn), pool),
        .generation = bytes_to_hex_string_pool(gs.get()->general.generation, GenerationLength, pool),
    };
}

// #define FK_TESTING_DOWNLOAD_LIMIT           (1024 * 1024 * 100)
// #define FK_TESTING_DOWNLOAD_READS_DISABLED
// #define FK_TESTING_DOWNLOAD_READS_GARBAGE

void DownloadWorker::run(Pool &pool) {
    serve(pool);
    connection_->busy(false);
}

void DownloadWorker::serve(Pool &pool) {
    loginfo("downloading");

    auto lock = storage_mutex.acquire(UINT32_MAX);
    FK_ASSERT(lock);

    auto started = fk_uptime();
    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory, pool };

    if (!storage.begin()) {
        connection_->error(HttpStatus::ServerError, "error opening storage", pool);
        return;
    }

    auto file_reader = storage.file_reader(file_number_, pool);

    auto is_head = connection_->is_head_method();
    auto maybe_info = get_headers(file_reader, pool);
    if (!maybe_info) {
        connection_->error(HttpStatus::ServerError, "error analysing storage", pool);
        return;
    }

    auto info = *maybe_info;

    if (info.first_block > info.last_block) {
        logwarn("range #%" PRIu32 " - #%" PRIu32 " size = %" PRIu32 " %s", info.first_block, info.last_block, info.size,
                is_head ? "HEAD" : "GET");
    } else {
        loginfo("range #%" PRIu32 " - #%" PRIu32 " size = %" PRIu32 " %s", info.first_block, info.last_block, info.size,
                is_head ? "HEAD" : "GET");
    }

    memory.log_statistics("flash usage: ");

#if defined(FK_TESTING_DOWNLOAD_LIMIT)
    FK_ASSERT(info.size > FK_TESTING_DOWNLOAD_LIMIT);
    info = HeaderInfo{
        .size = FK_TESTING_DOWNLOAD_LIMIT,
        .first_block = 0,
        .last_block = 0,
        .device_id = "none",
        .generation = "none",
    };
#endif

    if (!write_headers(info, pool)) {
        connection_->close();
        return;
    }

    if (is_head || info.size == 0) {
        connection_->close();
        return;
    }

    // Hello future programmer, you may be wondering why this is here
    // and even be tempted to remove this so you can chase a
    // problem. You will want to be very careful about this. This is
    // here, because if we log heavily during periods of high logging
    // activity the WiFi module starts to experience all kinds of
    // truama. My theory is that the logging gets in the way of the
    // IRQ handling. I believe you can even exacerbate this by moving
    // your AP further away so that latencies are higher and IRQs
    // happen during periods of more intense logging, say when
    // accessing the file system.
    // You've been warned.
    auto old_level = (LogLevels)log_get_level();
    log_configure_level(LogLevels::INFO);

    if ((LogLevels)log_get_level() != LogLevels::INFO) {
        logwarn("increased log verbosity will cause networking issues");
    }

    size_t buffer_size = NetworkBufferSize;
    uint8_t *buffer = (uint8_t *)pool.malloc(buffer_size);

    GlobalStateProgressCallbacks gs_progress;
    auto tracker = ProgressTracker{ &gs_progress, Operation::Download, "download", "", info.size };
    auto bytes_copied = 0u;
    auto total_read_time = 0u;
    auto total_write_time = 0u;
    while (bytes_copied < info.size) {
        auto to_read = std::min<int32_t>(buffer_size, info.size - bytes_copied);
        auto read_started = fk_uptime();
#if !defined(FK_TESTING_DOWNLOAD_READS_DISABLED)
#if !defined(FK_TESTING_DOWNLOAD_READS_GARBAGE)
        auto bytes_read = file_reader->read(buffer, to_read);
#else
        auto bytes_read = memory.read(0, buffer, to_read, MemoryReadFlags::None);
#endif
        if (bytes_read == 0) {
            break;
        }
        if (bytes_read < 0) {
            logerror("read error (%" PRId32 " != %" PRId32 ")", bytes_read, to_read);
            break;
        }
#else
        auto bytes_read = to_read;
        memset(buffer, 0xab, bytes_read);
#endif
        auto read_time = fk_uptime() - read_started;

        auto write_started = fk_uptime();
        auto wrote = connection_->write(buffer, bytes_read);
        if (wrote != bytes_read) {
            logerror("write error (%" PRId32 " != %" PRId32 ")", wrote, bytes_read);
            break;
        }

        auto write_time = fk_uptime() - write_started;

        tracker.update(bytes_read, read_time, write_time);

        bytes_copied += bytes_read;
        total_read_time += read_time;
        total_write_time += write_time;
    }

    log_configure_level(old_level);

    tracker.finished();

    auto elapsed = fk_uptime() - started;
    auto speed = ((bytes_copied / 1024.0f) / (elapsed / 1000.0f));
    loginfo("done (%d) (%" PRIu32 "ms) %.2fkbps total-read-time=%" PRIu32 " total-write-time=%" PRIu32, bytes_copied, elapsed, speed,
            total_read_time, total_write_time);

    connection_->close();

    memory.log_statistics("flash usage: ");
}

bool DownloadWorker::write_headers(HeaderInfo header_info, Pool &pool) {
    StackBufferedWriter<StackBufferSize> buffered{ connection_ };

    auto status = connection_->is_head_method() ? 204 : 200;

#define CHECK(expr)                                                                                                                        \
    if ((expr) == 0) {                                                                                                                     \
        return false;                                                                                                                      \
    }
    CHECK(buffered.write("HTTP/1.1 %d OK\n", status));
    CHECK(buffered.write("Content-Length: %" PRIu32 "\n", header_info.size));
    CHECK(buffered.write("Content-Type: %s\n", "application/octet-stream"));
    CHECK(buffered.write("Connection: close\n"));
    CHECK(buffered.write("Fk-Blocks: %" PRIu32 ", %" PRIu32 "\n", header_info.first_block, header_info.last_block));
    CHECK(buffered.write("Fk-Bytes: %" PRIu32 "\n", header_info.size));
    CHECK(buffered.write("Fk-DeviceId: %s\n", header_info.device_id));
    CHECK(buffered.write("Fk-Generation: %s\n\n", header_info.generation));

    auto flushed = buffered.flush();

    loginfo("flushed %d", flushed);

    return true;
}

DownloadHandler::DownloadHandler(uint8_t file_number) : file_number_(file_number) {
}

bool DownloadHandler::handle(HttpServerConnection *connection, Pool &pool) {
    auto worker = create_pool_worker<DownloadWorker>(connection, file_number_);
    get_ipc()->launch_worker(WorkerCategory::Transfer, worker);
    return true;
}

} // namespace fk
