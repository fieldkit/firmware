#pragma once

#include "worker.h"
#include "lora_manager.h"

namespace fk {

enum class LoraWorkOperation {
    FactoryReset,
    Configure,
    Readings,
};

struct LoraWork {
    LoraWorkOperation op;
};

class LoraWorker : public Worker {
private:
    LoraWork work_;

public:
    explicit LoraWorker(LoraWork work);

public:
    void run(Pool &pool) override;

    uint8_t priority() const override {
        return FK_PRIORITY_NORMAL;
    }

    const char *name() const override {
        return "lora";
    }

private:
    bool factory_reset(LoraManager &lora, Pool &pool);
    bool configure(LoraManager &lora, Pool &pool);
    bool readings(LoraManager &lora, Pool &pool);
};

FK_ENABLE_TYPE_NAME(LoraWorker);

} // namespace fk
