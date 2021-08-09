#pragma once

#include <modules/bridge/modules.h>

#include "ads_1219.h"
#include "mcp_2803.h"
#include "records.h"

namespace fk {

class WaterModule : public Module {
private:
    Pool *pool_{ nullptr };
    EncodedMessage *cfg_message_{ nullptr };
    fk_data_ModuleConfiguration *cfg_{ nullptr };
    ModuleHeader header_;

public:
    WaterModule(Pool &pool);
    virtual ~WaterModule();

public:
    ModuleReturn initialize(ModuleContext mc, Pool &pool) override;
    ModuleReturn service(ModuleContext mc, Pool &pool) override;
    ModuleReturn api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) override;
    ModuleReadings *take_readings(ReadingsContext mc, Pool &pool) override;

public:
    ModuleSensors const *get_sensors(Pool &pool) override;
    ModuleConfiguration const get_configuration(Pool &pool) override;

private:
    const char *get_display_name_key();
    bool load_configuration(ModuleContext mc, Pool &pool);

private:
    bool excite_control(Mcp2803 &mcp, bool high);
    bool initialize(Mcp2803 &mcp, Ads1219 &ads);
    uint32_t excite_duration();
};

} // namespace fk
