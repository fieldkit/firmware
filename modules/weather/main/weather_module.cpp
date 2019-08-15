#include "weather_module.h"

using namespace fk;

bool WeatherModule::initialize(ModuleContext mc, fk::Pool &pool) {
    return true;
}

static SensorMetadata const fk_module_weather_sensor_metas[] = {
    { .name = "temperature 1", .unitOfMeasure = "C" },
    { .name = "temperature 2", .unitOfMeasure = "C" },
    { .name = "pressure",      .unitOfMeasure = "kPa" },
    { .name = "humidity",      .unitOfMeasure = "%" },
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
