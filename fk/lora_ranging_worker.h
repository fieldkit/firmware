#pragma once

#include "worker.h"

namespace fk {

class LoraRangingWorker {
private:
    bool confirmed_;

public:
    LoraRangingWorker(bool confirmed);

public:
    void run(Pool &pool);

    uint8_t priority() const {
        return OS_PRIORITY_NORMAL + 2;
    }

    const char *name() {
        return "lora-range";
    }

};

}
