#pragma once

#include "worker.h"
#include "storage/storage.h"

namespace fk {

class DumpFlashMemory : public Worker {
public:
    void run(Pool &pool) override;

public:
    uint8_t priority() const override {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() override {
        return "dumpmem";
    }

};

}
