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

class DownloadLogsWorker {
private:
    HttpServerConnection *connection_;

public:
    DownloadLogsWorker(HttpServerConnection *connection);

public:
    void run(Pool &pool);

    uint8_t priority() const {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() {
        return "sendlogs";
    }

private:
    struct HeaderInfo {
        uint32_t size;
        const char *device_id;
        const char *generation;
    };

    bool write_headers(HeaderInfo header_info);

    HeaderInfo get_headers(Pool &pool);

};

}
