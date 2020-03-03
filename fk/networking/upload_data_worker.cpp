#include "networking/upload_data_worker.h"
#include "networking/http_connection.h"
#include "state_ref.h"
#include "utilities.h"
#include "progress_tracker.h"
#include "gs_progress_callbacks.h"
#include "storage/storage.h"

namespace fk {

FK_DECLARE_LOGGER("upload");

UploadDataWorker::UploadDataWorker() {
}

UploadDataWorker::UploadDataWorker(bool all_meta, bool all_data) : all_meta_(all_meta), all_data_(all_data) {
}

struct ConnectionInfo {
    const char *url;
    const char *token;
    const char *headers;
};

ConnectionInfo build_connection_info(uint32_t first, uint32_t last, uint32_t length, const char *type, Pool &pool) {
    fk_serial_number_t sn;
    auto gs = get_global_state_ro();
    auto headers = pool.sprintf(
        "Authorization: Bearer %s\r\n"
        "Content-Type: application/vnd.fk.data+binary\r\n"
        "Content-Length: %" PRIu32 "\r\n"
        "Fk-DeviceID: %s\r\n"
        "Fk-Generation: %s\r\n"
        "Fk-DeviceName: %s\r\n"
        "Fk-Blocks: %" PRIu32 ",%" PRIu32 "\r\n"
        "Fk-Type: %s\r\n",
        gs.get()->transmission.token,
        length,
        bytes_to_hex_string_pool((uint8_t *)&sn, sizeof(sn), pool),
        bytes_to_hex_string_pool((uint8_t *)&gs.get()->general.generation, sizeof(gs.get()->general.generation), pool),
        gs.get()->general.name,
        first,
        last,
        type
    );

    return {
        gs.get()->transmission.url,
        gs.get()->transmission.token,
        headers,
    };
}

UploadDataWorker::FileUpload UploadDataWorker::upload_file(Storage &storage, uint8_t file_number, uint32_t first_record, const char *type, Pool &pool) {
    auto started = fk_uptime();
    auto file = storage.file(file_number);
    if (false) {
        if (!file.seek_end()) {
            return { 0 };
        }
    }

    auto first_block = first_record;
    auto size_info = file.get_size(first_block, UINT32_MAX, pool);
    auto upload_length = size_info.size;
    auto last_block = size_info.last_block;
    if (upload_length == 0) {
        return { 0 };
    }

    auto connection_info = build_connection_info(first_block, last_block, upload_length, type, pool);
    if (strlen(connection_info.url) == 0 || strlen(connection_info.token) == 0) {
        return { 0 };
    }

    loginfo("uploading %" PRIu32 " -> %" PRIu32 " %" PRIu32 " bytes", first_block, last_block, upload_length);

    auto http = open_http_connection("POST", connection_info.url, connection_info.headers, false, pool);
    if (http == nullptr) {
        logwarn("unable to open connection");
        return { 0 };
    }

    auto buffer = (uint8_t *)pool.malloc(NetworkBufferSize);
    auto bytes_copied = 0u;

    loginfo("uploading file...");

    GlobalStateProgressCallbacks gs_progress;
    auto tracker = ProgressTracker{ &gs_progress, Operation::Upload, "upload", "", upload_length };
    while (bytes_copied != upload_length) {
        auto to_read = std::min<int32_t>(NetworkBufferSize, upload_length - bytes_copied);
        auto bytes_read = file.read(buffer, to_read);
        if (bytes_read != to_read) {
            logwarn("read error (%" PRId32 " != %" PRId32 ")", bytes_read, to_read);
            break;
        }

        auto bytes_wrote = http->write(buffer, to_read);
        if (bytes_wrote != (int32_t)to_read) {
            logwarn("write error (%" PRId32 " != %" PRId32 ")", bytes_wrote, to_read);
            break;
        }

        tracker.update(bytes_read);

        bytes_copied += bytes_read;
    }

    auto elapsed = fk_uptime() - started;
    auto speed = ((bytes_copied / 1024.0f) / (elapsed / 1000.0f));
    loginfo("done (%d) (%" PRIu32 "ms) %.2fkbps, waiting response", bytes_copied, elapsed, speed);

    if (!http->read_response()) {
        loginfo("unable to read response");
    }
    else {
        loginfo("http status %" PRId32, http->status_code());
    }

    http->close();

    return { last_block };
}

struct FileRecords {
    uint32_t meta;
    uint32_t data;
};

static FileRecords get_start_records() {
    auto gs = get_global_state_ro();
    return {
        gs.get()->transmission.meta_cursor,
        gs.get()->transmission.data_cursor,
    };
}

static void update_after_upload(FileRecords start_records) {
    auto gs = get_global_state_rw();

    gs.get()->transmission.meta_cursor = start_records.meta;
    gs.get()->transmission.data_cursor = start_records.data;
}

void UploadDataWorker::run(Pool &pool) {
    if (!get_network()->enabled()) {
        return;
    }

    auto lock = storage_mutex.acquire(UINT32_MAX);

    auto start_records = get_start_records();
    if (all_meta_) {
        start_records.meta = 0;
    }
    if (all_data_) {
        start_records.data = 0;
    }

    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory, pool };

    if (!storage.begin()) {
        return;
    }

    auto after = start_records;

    auto meta_upload = upload_file(storage, Storage::Meta, start_records.meta, "meta", pool);
    if (meta_upload) {
        after.meta = meta_upload.record + 1;
    }

    auto data_upload = upload_file(storage, Storage::Data, start_records.data, "data", pool);
    if (data_upload) {
        after.data = data_upload.record + 1;
    }

    update_after_upload(after);
}

}
