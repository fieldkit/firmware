#pragma once

#include "httpd/httpd.h"
#include "worker.h"

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

};

}
