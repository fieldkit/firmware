#pragma once

#include "worker.h"

namespace fk {

constexpr uint8_t AllModuleBays = 0xff;

enum class ConfigureModuleKind {
    Weather,
    Water,
    Ultrasonic,
};

class ConfigureModuleWorker : public Worker {
private:
    uint8_t bay_;
    ConfigureModuleKind kind_;

public:
    ConfigureModuleWorker(uint8_t bay, ConfigureModuleKind kind);

public:
    void run(Pool &pool) override;

public:
    uint8_t priority() const override {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() override {
        return "modcfg";
    }

private:
    bool configure(Pool &pool);
    bool scan(Pool &pool);

};

}
