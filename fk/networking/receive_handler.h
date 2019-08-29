#pragma once

#include "networking/networking.h"
#include "worker.h"

namespace fk {

class ReceiveHandler : public HttpHandler {
public:
    bool handle(HttpRequest &req, Pool &pool) override;

};

class ReceiveWorker : public Worker {
private:
    HttpRequest *req_;

public:
    ReceiveWorker(HttpRequest *req, Pool *pool);

public:
    void run(Pool &pool) override;

};

}
