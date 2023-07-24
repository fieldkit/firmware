#pragma once

#include <modules/bridge/modules.h>

#include "ads_1219.h"
#include "mcp_2803.h"
#include "curves.h"
#include "records.h"
#include "water_protocol.h"
#include "module_lockout.h"

namespace fk {

class WaterModule : public Module {
private:
    ModuleHeader header_;
    ModuleLockout lockout_;
    WaterConfig cfg_;

public:
    WaterModule(Pool &pool);
    virtual ~WaterModule();

public:
    ModuleReturn initialize(ModuleContext mc, Pool &pool) override;
    ModuleReturn service(ModuleContext mc, Pool &pool) override;
    ModuleReturn api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) override;
    ModuleReadings *take_readings(ReadingsContext mc, Pool &pool) override;
    bool can_enable() override;

public:
    ModuleSensors const *get_sensors(Pool &pool) override;
    ModuleConfiguration const get_configuration(Pool &pool) override;

private:
    const char *get_display_name_key();
    bool load_configuration(ModuleContext mc, Pool &pool);

private:
    WaterModality get_modality() const;
};

} // namespace fk
