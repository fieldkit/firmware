#pragma once

#include "worker.h"

namespace fk {

class CompareBanksWorker {
public:
    CompareBanksWorker();

public:
    void run(Pool &pool);

public:
    uint8_t priority() const {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() {
        return "cmpbanks";
    }

};

}
