#include "ultrasonic_module.h"
#include "platform.h"

using namespace fk;

FK_DECLARE_LOGGER("ultrasonic");

bool UltrasonicModule::initialize(ModuleContext mc, fk::Pool &pool) {
    return true;
}

static SensorMetadata const fk_module_ultrasonic_sensor_metas[] = {
    { .name = "calibration", .unitOfMeasure = "cm", .flags = 0 },
    { .name = "distance",    .unitOfMeasure = "cm", .flags = 0 },
};

static ModuleSensors fk_module_ultrasonic_sensors = {
    .nsensors = sizeof(fk_module_ultrasonic_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_ultrasonic_sensor_metas,
};

ModuleSensors const *UltrasonicModule::get_sensors(Pool &pool) {
    return &fk_module_ultrasonic_sensors;
}

ModuleReadings *UltrasonicModule::take_readings(ModuleContext mc, fk::Pool &pool) {
    return nullptr;
}
