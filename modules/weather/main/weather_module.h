#pragma once

#include <modules.h>

class WeatherModule : public fk::Module {
public:
    bool initialize(fk::ModuleContext mc, fk::Pool &pool) override;
    fk::ModuleSensors const *get_sensors(fk::ModuleContext mc, fk::Pool &pool) override;
    fk::ModuleReadings *take_readings(fk::ModuleContext mc, fk::Pool &pool) override;

};
