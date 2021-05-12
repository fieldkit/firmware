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

    const char *name() const override {
        return "refmodules";
    }

    TaskDisplayInfo display_info() const override {
        return {
            .name = name(),
            .progress = 0.0f,
            .visible = false,
        };
    }

};

FK_ENABLE_TYPE_NAME(RefreshModulesWorker);

}
