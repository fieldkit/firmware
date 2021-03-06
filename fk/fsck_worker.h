#pragma once

#include "worker.h"

namespace fk {

class FsckWorker : public Worker {
public:
    void run(Pool &pool) override;

    uint8_t priority() const override {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() const override {
        return "fsck";
    }

};

FK_ENABLE_TYPE_NAME(FsckWorker);

}
