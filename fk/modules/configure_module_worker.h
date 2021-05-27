#pragma once

#include "worker.h"
#include "modules/shared/modules.h"

namespace fk {

class ConfigureModuleWorker : public Worker {
private:
    ModulePosition bay_;
    bool erase_{ false };
    ModuleHeader header_;

public:
    ConfigureModuleWorker(ModulePosition bay);
    ConfigureModuleWorker(ModulePosition bay, ModuleHeader header);

public:
    void run(Pool &pool) override;

public:
    const char *name() const override {
        return "modcfg";
    }

private:
    bool configure(Pool &pool);
    bool scan(Pool &pool);

};

FK_ENABLE_TYPE_NAME(ConfigureModuleWorker);

}
