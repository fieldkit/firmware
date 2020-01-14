#pragma once

#include <modules/bridge/modules.h>

class RandomModule : public fk::Module {
public:
    fk::ModuleReturn initialize(fk::ModuleContext mc, fk::Pool &pool) override;
    fk::ModuleReturn service(fk::ModuleContext mc, fk::Pool &pool) override;
    fk::ModuleReturn api(fk::ModuleContext mc, fk::HttpServerConnection *connection, fk::Pool &pool) override;
    fk::ModuleReadings *take_readings(fk::ModuleContext mc, fk::Pool &pool) override;

public:
    fk::ModuleSensors const *get_sensors(fk::Pool &pool) override;
    fk::ModuleConfiguration get_configuration(fk::Pool &pool) override;

};
