#pragma once

#include <modules/bridge/modules.h>

#include "mcp_2803.h"
#include "ads_1219.h"
#include "curves.h"
#include "records.h"
#include "module_lockout.h"

namespace fk {

class OmniWaterModule : public Module {
private:
    Pool *pool_{ nullptr };
    EncodedMessage *cfg_message_{ nullptr };
    fk_data_ModuleConfiguration *cfg_{ nullptr };
    ModuleHeader header_;
    ModuleLockout lockout_;

public:
    OmniWaterModule(Pool &pool);
    virtual ~OmniWaterModule();

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
    bool load_configuration(ModuleContext mc, Pool &pool);

private:
    bool initialize(Mcp2803 &mcp, Ads1219 &ads);
};

} // namespace fk
