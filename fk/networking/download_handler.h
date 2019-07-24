#pragma once

#include "worker.h"
#include "networking/networking.h"

namespace fk {

class DownloadHandler : public HttpHandler {
public:
    bool handle(HttpRequest &req) override;

};

class DownloadWorker : public Worker {
private:
    HttpRequest *req_;

public:
    DownloadWorker(HttpRequest *req);

public:
    void run(WorkerContext &wc) override;

private:
    struct HeaderInfo {
        uint32_t size;
        uint32_t first_block;
        uint32_t last_block;
    };

    bool write_headers(HeaderInfo header_info);

};

}
