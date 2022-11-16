#pragma once

#include <modules/bridge/modules.h>

namespace fk {

class WaterDepthModule : public Module {
private:
    Pool *pool_{ nullptr };

public:
    WaterDepthModule(Pool &pool);
    virtual ~WaterDepthModule();

public:
    ModuleReturn initialize(ModuleContext mc, Pool &pool) override;
    ModuleReturn service(ModuleContext mc, Pool &pool) override;
    ModuleReturn api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) override;
    ModuleReadings *take_readings(ReadingsContext mc, Pool &pool) override;
    bool can_enable() override {
        return true;
    }

public:
    ModuleSensors const *get_sensors(Pool &pool) override;
    ModuleConfiguration const get_configuration(Pool &pool) override;
};

} // namespace fk
