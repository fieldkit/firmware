#pragma once

#include "worker.h"
#include "storage/storage.h"

namespace fk {

class DumpFlashMemory {
public:
    void run(Pool &pool);

public:
    uint8_t priority() const {
        return OS_PRIORITY_NORMAL;
    }

    const char *name() {
        return "dumpmem";
    }

};

}
