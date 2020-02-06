#include "aggregated_weather.h"

#include "weather.h"
#include "clock.h"

namespace fk {

FK_DECLARE_LOGGER("weather");

ModuleReturn AggregatedWeather::initialize(ModuleContext mc, Pool &pool) {
    auto &bus = mc.module_bus();

    if (!I2C_CHECK(bus.write_register_u32(FK_WEATHER_I2C_ADDRESS, FK_WEATHER_I2C_COMMAND_CONFIG, get_clock_now()))) {
        return { ModuleStatus::Fatal };
    }

    return { ModuleStatus::Ok };
}

ModuleReturn AggregatedWeather::service(ModuleContext mc, Pool &pool) {
    return { ModuleStatus::Ok };
}

ModuleReadings *AggregatedWeather::take_readings(ModuleContext mc, Pool &pool) {
    auto &bus = mc.module_bus();

    fk_weather_aggregated_t aw;
    memzero(&aw, sizeof(aw));
    if (!I2C_CHECK(bus.read_register_buffer(FK_WEATHER_I2C_ADDRESS, FK_WEATHER_I2C_COMMAND_READ, (uint8_t *)&aw, sizeof(aw)))) {
        return nullptr;
    }

    auto mr = new (pool) NModuleReadings<16>();
    for (auto i = 0u; i < 16u; ++i) {
        mr->set(i, 0.0f);
    }

    return mr;
}

} // namespace fk
