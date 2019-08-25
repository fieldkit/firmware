#pragma once

#include "worker.h"
#include "networking/networking.h"

namespace fk {

class DownloadHandler : public HttpHandler {
private:
    uint8_t file_number_;

public:
    DownloadHandler(uint8_t file_number);

public:
    bool handle(HttpRequest &req, Pool &pool) override;

};

class DownloadWorker : public Worker {
private:
    HttpRequest *req_;
    uint8_t file_number_;

public:
    DownloadWorker(HttpRequest *req, uint8_t file_number);

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
