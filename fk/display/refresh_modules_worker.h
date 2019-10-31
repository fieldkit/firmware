#pragma once

#include "worker.h"

namespace fk {

class RefreshModulesWorker : public Worker {
public:
    RefreshModulesWorker();

public:
    void run(Pool &pool) override;

public:
    uint8_t priority() const override {
        return OS_PRIORITY_NORMAL;
    }

    const char *name() override {
        return "refmodules";
    }

};

}