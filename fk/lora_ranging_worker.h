#pragma once

#include "worker.h"

namespace fk {

class LoraRangingWorker : public Worker {
private:
    bool confirmed_;

public:
    LoraRangingWorker(bool confirmed);

public:
    void run(Pool &pool) override;

    uint8_t priority() const  override{
        return OS_PRIORITY_NORMAL + 2;
    }

    const char *name() const override{
        return "lora-range";
    }

};

FK_ENABLE_TYPE_NAME(LoraRangingWorker);

}
