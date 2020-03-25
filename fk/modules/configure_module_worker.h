#pragma once

#include "worker.h"
#include "modules/shared/modules.h"

namespace fk {

constexpr uint8_t AllModuleBays = 0xff;

class ConfigureModuleWorker : public Worker {
private:
    uint8_t bay_;
    ModuleHeader header_;

public:
    ConfigureModuleWorker(uint8_t bay, ModuleHeader header);

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

FK_ENABLE_TYPE_NAME(ConfigureModuleWorker);

}
