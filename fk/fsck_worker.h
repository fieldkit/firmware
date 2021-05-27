#pragma once

#include "worker.h"

namespace fk {

class FsckWorker : public Worker {
public:
    void run(Pool &pool) override;

    const char *name() const override {
        return "fsck";
    }

};

FK_ENABLE_TYPE_NAME(FsckWorker);

}
