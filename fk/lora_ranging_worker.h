#pragma once

#include "worker.h"

namespace fk {

class LoraRangingWorker {
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
