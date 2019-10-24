#pragma once

#include "networking/networking.h"
#include "worker.h"

namespace fk {

class ReceiveFirmwareHandler : public HttpHandler {
public:
    bool handle(Connection *connection, Pool &pool) override;

};

class ReceiveFirmwareWorker {
private:
    Connection *connection_;

public:
    ReceiveFirmwareWorker(Connection *connection);

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
