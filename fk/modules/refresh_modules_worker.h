#pragma once

#include "worker.h"

namespace fk {

class RefreshModulesWorker : public Worker {
private:
    bool readings_{ true };

public:
    RefreshModulesWorker(bool readings = true);

public:
    void run(Pool &pool) override;

public:
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

} // namespace fk
