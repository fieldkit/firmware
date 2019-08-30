#pragma once

#include "networking/networking.h"
#include "worker.h"

namespace fk {

class ReceiveHandler : public HttpHandler {
public:
    bool handle(HttpRequest &req, Pool &pool) override;

};

class ReceiveWorker {
private:
    HttpRequest *req_;

public:
    ReceiveWorker(HttpRequest &req, uint32_t a);

public:
    void run(Pool &pool);

    uint8_t priority() const {
        return OS_PRIORITY_NORMAL;
    }

    const char *name() {
        return "receive";
    }

};

}
