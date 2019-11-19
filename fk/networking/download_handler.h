#pragma once

#include "worker.h"
#include "networking/networking.h"
#include "storage/storage.h"

namespace fk {

class DownloadHandler : public HttpHandler {
private:
    uint8_t file_number_;

public:
    DownloadHandler(uint8_t file_number);

public:
    bool handle(HttpServerConnection *connection, Pool &pool) override;

};

class DownloadWorker {
private:
    HttpServerConnection *connection_;
    uint8_t file_number_;

public:
    DownloadWorker(HttpServerConnection *connection, uint8_t file_number);

public:
    void run(Pool &pool);

    uint8_t priority() const {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() {
        return "download";
    }

private:
    struct HeaderInfo {
        uint32_t size;
        uint32_t first_block;
        uint32_t last_block;
        const char *device_id;
        const char *generation;
    };

    bool write_headers(HeaderInfo header_info);

    HeaderInfo get_headers(File &file, Pool &pool);

};

}
