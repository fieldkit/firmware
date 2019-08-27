#include "weather_module.h"

using namespace fk;

bool WeatherModule::initialize(ModuleContext mc, fk::Pool &pool) {
    return true;
}

static SensorMetadata const fk_module_weather_sensor_metas[] = {
    { .name = "temperature 1", .unitOfMeasure = "C",   .flags = 0 },
    { .name = "temperature 2", .unitOfMeasure = "C",   .flags = 0 },
    { .name = "pressure",      .unitOfMeasure = "kPa", .flags = 0 },
    { .name = "humidity",      .unitOfMeasure = "%",   .flags = 0 },
};

static ModuleSensors fk_module_weather_sensors = {
    .nsensors = sizeof(fk_module_weather_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_weather_sensor_metas,
};

ModuleSensors const *WeatherModule::get_sensors(ModuleContext mc, Pool &pool) {
    return &fk_module_weather_sensors;
}

ModuleReadings *WeatherModule::take_readings(ModuleContext mc, fk::Pool &pool) {
    return nullptr;
}
