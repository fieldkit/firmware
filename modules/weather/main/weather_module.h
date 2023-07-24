#pragma once

#include <modules/bridge/modules.h>

#include "weather.h"
#include "aggregated_weather.h"
#include "weather_types.h"

namespace fk {

class WeatherModule : public Module {
private:
    AggregatedWeather delegate_;

public:
    WeatherModule(Pool &pool);
    virtual ~WeatherModule();

public:
    ModuleReturn initialize(ModuleContext mc, Pool &pool) override;
    ModuleReturn service(ModuleContext mc, Pool &pool) override;
    ModuleReturn api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) override;
    ModuleReadings *take_readings(ReadingsContext mc, Pool &pool) override;

public:
    ModuleSensors const *get_sensors(Pool &pool) override;
    ModuleConfiguration const get_configuration(Pool &pool) override;
};

} // namespace fk
