#pragma once

#include <modules/bridge/modules.h>

#include "weather_types.h"

namespace fk {

class AggregatedWeather {
public:
    ModuleReturn initialize(ModuleContext mc, Pool &pool);
    ModuleReturn service(ModuleContext mc, Pool &pool);
    ModuleReadings *take_readings(ModuleContext mc, Pool &pool);

};

} // namespace fk
