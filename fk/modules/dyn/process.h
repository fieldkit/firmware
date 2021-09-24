#pragma once

#include "worker.h"

namespace fk {

class Process : public Worker {
public:
    void run(Pool &pool) override;

    TaskDisplayInfo display_info() const override {
        return { .name = "", .progress = 0.0f, .visible = false };
    }

public:
    const char *name() const override {
        return "process";
    }

};

FK_ENABLE_TYPE_NAME(Process);

} // namespace fk
