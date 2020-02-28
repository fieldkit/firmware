#include "networking/upload_data_worker.h"
#include "networking/http_connection.h"

namespace fk {

FK_DECLARE_LOGGER("upload");

UploadDataWorker::UploadDataWorker() {
}

void UploadDataWorker::run(Pool &pool) {
    auto url = "http://192.168.0.100:8080/status";
    auto http = open_http_connection("GET", url, pool);
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
            loginfo("break");
            break;
        }

        total_read += nread;

        buffer[nread] = 0;
        SEGGER_RTT_WriteString(0, (char *)buffer);
    }

    loginfo("done (%u bytes)", total_read);

    http->close();
}

}
