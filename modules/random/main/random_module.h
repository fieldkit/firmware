#pragma once

#include <modules/bridge/modules.h>

namespace fk {

class RandomModule : public Module {
public:
    ModuleReturn initialize(ModuleContext mc, Pool &pool) override;
    ModuleReturn service(ModuleContext mc, Pool &pool) override;
    ModuleReturn api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) override;
    ModuleStatusReturn status(ModuleContext mc, Pool &pool) override;
    ModuleReadings *take_readings(ReadingsContext mc, Pool &pool) override;

public:
    ModuleSensors const *get_sensors(Pool &pool) override;
    ModuleConfiguration get_configuration(Pool &pool) override;
};

} // namespace fk
