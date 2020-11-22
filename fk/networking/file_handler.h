#pragma once

#include "worker.h"
#include "networking/networking.h"
#include "storage/storage.h"

namespace fk {

class DownloadFileHandler : public HttpHandler {
public:
    DownloadFileHandler();

public:
    bool handle(HttpServerConnection *connection, Pool &pool) override;

};

class DownloadFileWorker : public Worker {
private:
    HttpServerConnection *connection_;

public:
    DownloadFileWorker(HttpServerConnection *connection);

public:
    void run(Pool &pool) override;

    uint8_t priority() const override {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() const override {
        return "sendfile";
    }

private:
    struct HeaderInfo {
        uint32_t size;
        const char *device_id;
        const char *generation;
    };

    bool write_headers(HeaderInfo header_info);

    HeaderInfo get_headers(SdCardFile *file, Pool &pool);

};

FK_ENABLE_TYPE_NAME(DownloadFileWorker);

}
