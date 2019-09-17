#pragma once

#include <modules/bridge/modules.h>

class RandomModule : public fk::Module {
public:
    bool initialize(fk::ModuleContext mc, fk::Pool &pool) override;
    fk::ModuleReadings *take_readings(fk::ModuleContext mc, fk::Pool &pool) override;

public:
    fk::ModuleSensors const *get_sensors(fk::Pool &pool) override;

};
