#pragma once

#include "modules/bridge/modules.h"

namespace fk {

class UnknownModule : public Module {
public:
    ModuleReturn initialize(ModuleContext mc, Pool &pool) override;
    ModuleReturn api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) override;
    ModuleReturn service(ModuleContext mc, Pool &pool) override;
    ModuleSensors const *get_sensors(Pool &pool) override;
    ModuleConfiguration const get_configuration(Pool &pool) override;
    ModuleReadings *take_readings(ReadingsContext mc, Pool &pool) override;
};

extern ModuleMetadata const fk_unknown_module;

} // namespace fk
