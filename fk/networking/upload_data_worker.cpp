#include "networking/upload_data_worker.h"
#include "networking/http_connection.h"
#include "state_ref.h"
#include "utilities.h"

namespace fk {

FK_DECLARE_LOGGER("upload");

UploadDataWorker::UploadDataWorker() {
}

const char *build_headers(uint32_t first, uint32_t last, uint32_t length, Pool &pool) {
    fk_serial_number_t sn;
    auto gs = get_global_state_ro();
    return pool.sprintf(
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
        "data"
    );
}

void UploadDataWorker::run(Pool &pool) {
    auto extra_headers = build_headers(0, 0, 0, pool);
    auto url = "http://192.168.0.100:8080/ingestion";
    auto http = open_http_connection("POST", url, extra_headers, pool);
    if (http == nullptr) {
        return;
    }

    auto buffer = (uint8_t *)pool.malloc(CodeMemoryPageSize);
    auto total_read = 0u;

    loginfo("sending...");

    while (true) {
        auto reading = CodeMemoryPageSize - 1;
        auto nread = http->read(buffer, reading);
        if (nread < 0) {
            break;
        }

        total_read += nread;
    }

    loginfo("done (%u bytes)", total_read);

    http->close();
}

}
