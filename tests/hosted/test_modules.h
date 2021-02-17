#pragma once

#include "modules/bridge/modules_bridge.h"

namespace fk {

extern ModuleMetadata const fk_test_module_fake_random;
extern ModuleMetadata const fk_test_module_fake_diagnostics;
extern ModuleMetadata const fk_test_module_fake_1;
extern ModuleMetadata const fk_test_module_fake_2;
extern ModuleMetadata const fk_test_module_fake_empty;

class FakeModule : public Module {
};

class FakeModule1 : public FakeModule {
private:
    bool return_none_{ false };

public:
    ModuleReturn initialize(ModuleContext mc, Pool &pool) override {
        return { ModuleStatus::Ok };
    }

    ModuleReturn api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
        return { ModuleStatus::Ok };
    }

    ModuleReturn service(ModuleContext mc, Pool &pool) {
        return { ModuleStatus::Ok };
    }

    ModuleSensors const *get_sensors(Pool &pool) override;

    ModuleConfiguration const get_configuration(Pool &pool) override {
        return { "modules.fake.1" };
    }

    ModuleReadings *take_readings(ReadingsContext mc, Pool &pool) override;

    void return_none() {
        return_none_ = true;
    }
};

}
