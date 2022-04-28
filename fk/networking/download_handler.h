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

class DownloadWorker : public Worker {
private:
    HttpServerConnection *connection_;
    uint8_t file_number_;

public:
    DownloadWorker(HttpServerConnection *connection, uint8_t file_number);

public:
    void run(Pool &pool) override;

    const char *name() const override {
        return "download";
    }

private:
    void serve(Pool &pool);

    struct HeaderInfo {
        uint32_t size;
        uint32_t first_block;
        uint32_t last_block;
        const char *device_id;
        const char *generation;
    };

    bool write_headers(HeaderInfo header_info, Pool &pool);

    tl::expected<DownloadWorker::HeaderInfo, Error> get_headers(FileReader *file_reader, Pool &pool);
};

FK_ENABLE_TYPE_NAME(DownloadWorker);

} // namespace fk
