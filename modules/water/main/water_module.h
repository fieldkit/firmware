#pragma once

#include <modules/bridge/modules.h>

#include "records.h"
#include "mcp_2803.h"
#include "ads_1219.h"

namespace fk {

class WaterModule : public Module {
private:
    Pool *pool_{ nullptr };
    EncodedMessage *cfg_message_{ nullptr };
    fk_data_ModuleConfiguration *cfg_{ nullptr };

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
    bool excite(Mcp2803 &mcp, uint32_t cleanse_ms);

};

} // namespace fk
