#pragma once

#include "worker.h"

namespace fk {

class FactoryWipeWorker {
public:
    void run(Pool &pool);

    uint8_t priority() const {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() {
        return "wipe";
    }

};

}
