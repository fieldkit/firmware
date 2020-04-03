#pragma once

#include "networking/networking.h"
#include "worker.h"

namespace fk {

class ReceiveFirmwareHandler : public HttpHandler {
public:
    bool handle(HttpServerConnection *connection, Pool &pool) override;

};

class ReceiveFirmwareWorker : public Worker {
private:
    HttpServerConnection *connection_;

public:
    ReceiveFirmwareWorker(HttpServerConnection *connection);

public:
    void run(Pool &pool) override;

    uint8_t priority() const override {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() const override {
        return "uplfirmware";
    }

};

FK_ENABLE_TYPE_NAME(ReceiveFirmwareWorker);

}
