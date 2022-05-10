#pragma once

#include "worker.h"
#include "networking/networking.h"
#include "storage/storage.h"

namespace fk {

class DownloadLogsHandler : public HttpHandler {
public:
    DownloadLogsHandler();

public:
    bool handle(HttpServerConnection *connection, Pool &pool) override;
};

class DownloadLogsWorker : public Worker {
private:
    HttpServerConnection *connection_;

public:
    DownloadLogsWorker(HttpServerConnection *connection);

public:
    void run(Pool &pool) override;

    const char *name() const override {
        return "sendlogs";
    }

private:
    void serve(Pool &pool);

    struct HeaderInfo {
        uint32_t size;
        const char *device_id;
        const char *generation;
    };

    bool write_headers(HeaderInfo header_info);

    HeaderInfo get_headers(Pool &pool);
};

FK_ENABLE_TYPE_NAME(DownloadLogsWorker);

} // namespace fk
