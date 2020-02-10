#pragma once

#include <modules/bridge/modules.h>

#include "oem_atlas.h"

namespace fk {

class WaterModule : public Module {
private:
    AtlasSensorType type_{ AtlasSensorType::Unknown };
    uint8_t address_{ 0 };

public:
    ModuleReturn initialize(ModuleContext mc, Pool &pool) override;
    ModuleReturn service(ModuleContext mc, Pool &pool) override;
    ModuleReturn api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) override;
    ModuleReadings *take_readings(ModuleContext mc, Pool &pool) override;

public:
    ModuleSensors const *get_sensors(Pool &pool) override;
    ModuleConfiguration get_configuration(Pool &pool) override;

private:
    const char *get_display_name_key();

};

} // namespace fk
