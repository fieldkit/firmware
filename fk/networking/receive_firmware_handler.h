#pragma once

#include "networking/networking.h"
#include "worker.h"

namespace fk {

class ReceiveFirmwareHandler : public HttpHandler {
public:
    bool handle(HttpServerConnection *connection, Pool &pool) override;

};

class ReceiveFirmwareWorker {
private:
    HttpServerConnection *connection_;

public:
    ReceiveFirmwareWorker(HttpServerConnection *connection);

public:
    void run(Pool &pool);

    uint8_t priority() const {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() {
        return "uplfirmware";
    }

};

}
