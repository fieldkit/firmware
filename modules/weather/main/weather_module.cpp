#include "weather_module.h"
#include "utilities.h"
#include "modules/shared/crc.h"
#include "modules/eeprom.h"
#include "weather.h"
#include "platform.h"
#include "clock.h"

namespace fk {

FK_DECLARE_LOGGER("weather");

static SensorMetadata const fk_module_weather_sensor_metas[] = {
    { .name = "humidity",             .unitOfMeasure = "%",      .flags = 0 },
    { .name = "temperature_1",        .unitOfMeasure = "C",      .flags = 0 },
    { .name = "pressure",             .unitOfMeasure = "kPa",    .flags = 0 },
    { .name = "temperature_2",        .unitOfMeasure = "C",      .flags = 0 },

    { .name = "rain",                 .unitOfMeasure = "mm",     .flags = 0 },

    { .name = "wind_speed",           .unitOfMeasure = "km/hr",  .flags = 0 },
    { .name = "wind_dir",             .unitOfMeasure = "",       .flags = 0 },
    { .name = "wind_dir_mv",          .unitOfMeasure = "mv",     .flags = 0 },

    { .name = "wind_hr_max_speed",    .unitOfMeasure = "km/hr",  .flags = 0 },
    { .name = "wind_hr_max_dir",      .unitOfMeasure = "",       .flags = 0 },
    { .name = "wind_10m_max_speed",   .unitOfMeasure = "km/hr",  .flags = 0 },
    { .name = "wind_10m_max_dir",     .unitOfMeasure = "",       .flags = 0 },
    { .name = "wind_2m_avg_speed",    .unitOfMeasure = "km/hr",  .flags = 0 },
    { .name = "wind_2m_avg_dir",      .unitOfMeasure = "",       .flags = 0 },

    { .name = "rain_this_hour",       .unitOfMeasure = "mm",     .flags = 0 },
    { .name = "rain_prev_hour",       .unitOfMeasure = "mm",     .flags = 0 },
};

static ModuleSensors fk_module_weather_sensors = {
    .nsensors = sizeof(fk_module_weather_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_weather_sensor_metas,
};

ModuleSensors const *WeatherModule::get_sensors(Pool &pool) {
    return &fk_module_weather_sensors;
}

ModuleConfiguration WeatherModule::get_configuration(Pool &pool) {
    #if defined(FK_WEATHER_JOURNALED)
    return { "modules.weather", ModulePower::Always, 60 };
    #else
    return { "modules.weather" };
    #endif
}

ModuleReturn WeatherModule::initialize(ModuleContext mc, Pool &pool) {
    return delegate_.initialize(mc, pool);
}

ModuleStatusReturn WeatherModule::status(ModuleContext mc, Pool &pool) {
    return { ModuleStatus::Ok, nullptr };
}

ModuleReturn WeatherModule::api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
    connection->busy(0, "unsupported");

    return { ModuleStatus::Fatal };
}

ModuleReturn WeatherModule::service(ModuleContext mc, Pool &pool) {
    return delegate_.service(mc, pool);
}

ModuleReadings *WeatherModule::take_readings(ReadingsContext mc, Pool &pool) {
    return delegate_.take_readings(mc, pool);
}

} // namespace fk
