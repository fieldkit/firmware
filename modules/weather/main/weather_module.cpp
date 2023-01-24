#include "weather_module.h"
#include "utilities.h"
#include "modules/shared/crc.h"
#include "modules/eeprom.h"
#include "weather.h"
#include "platform.h"
#include "hal/clock.h"

namespace fk {

FK_DECLARE_LOGGER("weather");

static SensorMetadata const fk_module_weather_sensor_metas[] = {
    { .name = "humidity", .unitOfMeasure = "%", .uncalibratedUnitOfMeasure = "%", .flags = 0 },
    { .name = "temperature_1", .unitOfMeasure = "°C", .uncalibratedUnitOfMeasure = "°C", .flags = 0 },
    { .name = "pressure", .unitOfMeasure = "kPa", .uncalibratedUnitOfMeasure = "kPa", .flags = 0 },
    { .name = "temperature_2", .unitOfMeasure = "°C", .uncalibratedUnitOfMeasure = "°C", .flags = 0 },

    { .name = "rain", .unitOfMeasure = "mm", .uncalibratedUnitOfMeasure = "mm", .flags = 0 },

    { .name = "wind_speed", .unitOfMeasure = "km/hr", .uncalibratedUnitOfMeasure = "km/hr", .flags = 0 },
    { .name = "wind_dir", .unitOfMeasure = "°", .uncalibratedUnitOfMeasure = "°", .flags = 0 },
    { .name = "wind_dir_mv", .unitOfMeasure = "mV", .uncalibratedUnitOfMeasure = "mV", .flags = 0 },

    { .name = "wind_hr_max_speed", .unitOfMeasure = "km/hr", .uncalibratedUnitOfMeasure = "km/hr", .flags = 0 },
    { .name = "wind_hr_max_dir", .unitOfMeasure = "°", .uncalibratedUnitOfMeasure = "°", .flags = 0 },
    { .name = "wind_10m_max_speed", .unitOfMeasure = "km/hr", .uncalibratedUnitOfMeasure = "km/hr", .flags = 0 },
    { .name = "wind_10m_max_dir", .unitOfMeasure = "°", .uncalibratedUnitOfMeasure = "°", .flags = 0 },
    { .name = "wind_2m_avg_speed", .unitOfMeasure = "km/hr", .uncalibratedUnitOfMeasure = "km/hr", .flags = 0 },
    { .name = "wind_2m_avg_dir", .unitOfMeasure = "°", .uncalibratedUnitOfMeasure = "°", .flags = 0 },

    { .name = "rain_this_hour", .unitOfMeasure = "mm", .uncalibratedUnitOfMeasure = "mm", .flags = 0 },
    { .name = "rain_prev_hour", .unitOfMeasure = "mm", .uncalibratedUnitOfMeasure = "mm", .flags = 0 },
};

static ModuleSensors fk_module_weather_sensors = {
    .nsensors = sizeof(fk_module_weather_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_weather_sensor_metas,
};

WeatherModule::WeatherModule(Pool &pool) {
}

WeatherModule::~WeatherModule() {
}

ModuleSensors const *WeatherModule::get_sensors(Pool &pool) {
    return &fk_module_weather_sensors;
}

ModuleConfiguration const WeatherModule::get_configuration(Pool &pool) {
    return ModuleConfiguration{ "modules.weather", ModulePower::Always };
}

ModuleReturn WeatherModule::initialize(ModuleContext mc, Pool &pool) {
    return delegate_.initialize(mc, pool);
}

ModuleReturn WeatherModule::api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
    connection->busy(0, "unsupported", pool);

    return { ModuleStatus::Fatal };
}

ModuleReturn WeatherModule::service(ModuleContext mc, Pool &pool) {
    return delegate_.service(mc, pool);
}

ModuleReadings *WeatherModule::take_readings(ReadingsContext mc, Pool &pool) {
    return delegate_.take_readings(mc, pool);
}

} // namespace fk
