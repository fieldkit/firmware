#pragma once

#include "worker.h"

namespace fk {

class LoraWorker : public Worker {
public:
    void run(Pool &pool) override;

    uint8_t priority() const override {
        return OS_PRIORITY_NORMAL + 2;
    }

    const char *name() const override {
        return "lora";
    }

};

FK_ENABLE_TYPE_NAME(LoraWorker);

}
