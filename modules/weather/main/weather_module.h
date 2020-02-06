#pragma once

#include <modules/bridge/modules.h>

#include "weather.h"
#include "aggregated_weather.h"
#include "journaled_weather.h"
#include "weather_types.h"

namespace fk {

class WeatherModule : public Module {
private:
    #if defined(FK_WEATHER_JOURNALED)
    JournaledWeather delegate_;
    #else
    AggregatedWeather delegate_;
    #endif

public:
    ModuleReturn initialize(ModuleContext mc, Pool &pool) override;
    ModuleReturn service(ModuleContext mc, Pool &pool) override;
    ModuleReturn api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) override;
    ModuleReadings *take_readings(ModuleContext mc, Pool &pool) override;

public:
    ModuleSensors const *get_sensors(Pool &pool) override;
    ModuleConfiguration get_configuration(Pool &pool) override;

};

}
