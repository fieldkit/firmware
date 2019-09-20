#pragma once

#include <modules/bridge/modules.h>

class WeatherModule : public fk::Module {
private:
    uint32_t seconds_{ 0 };
    uint32_t session_{ 0 };
    uint32_t address_{ 0 };

public:
    bool initialize(fk::ModuleContext mc, fk::Pool &pool) override;
    fk::ModuleReadings *take_readings(fk::ModuleContext mc, fk::Pool &pool) override;

public:
    fk::ModuleSensors const *get_sensors(fk::Pool &pool) override;

};
