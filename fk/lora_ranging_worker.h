#pragma once

#include "worker.h"
#include "lora_manager.h"

namespace fk {

class LoraRangingWorker : public Worker {
public:
    explicit LoraRangingWorker();

public:
    void run(Pool &pool) override;

    const char *name() const override {
        return "lora-range";
    }

private:
    void loop(LoraManager &lora);
};

FK_ENABLE_TYPE_NAME(LoraRangingWorker);

} // namespace fk
