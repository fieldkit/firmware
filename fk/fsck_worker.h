#pragma once

#include "worker.h"

namespace fk {

class FsckWorker {
public:
    void run(Pool &pool);

    uint8_t priority() const {
        return OS_PRIORITY_NORMAL;
    }

    const char *name() {
        return "fsck";
    }

};

}
