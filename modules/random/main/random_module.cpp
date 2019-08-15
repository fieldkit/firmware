#include "random_module.h"

using namespace fk;

bool RandomModule::initialize(ModuleContext mc, fk::Pool &pool) {
    return true;
}

static SensorMetadata const fk_module_random_sensor_metas[] = {
    { .name = "random-0", .unitOfMeasure = "" },
    { .name = "random-1", .unitOfMeasure = "" },
    { .name = "random-2", .unitOfMeasure = "" },
    { .name = "random-3", .unitOfMeasure = "" },
    { .name = "random-4", .unitOfMeasure = "" },
    { .name = "random-5", .unitOfMeasure = "" },
    { .name = "random-6", .unitOfMeasure = "" },
    { .name = "random-7", .unitOfMeasure = "" },
    { .name = "random-8", .unitOfMeasure = "" },
    { .name = "random-9", .unitOfMeasure = "" },
};

static ModuleSensors fk_module_random_sensors = {
    .nsensors = sizeof(fk_module_random_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_random_sensor_metas,
};

ModuleSensors const *RandomModule::get_sensors(ModuleContext mc, Pool &pool) {
    return &fk_module_random_sensors;
}

ModuleReadings *RandomModule::take_readings(ModuleContext mc, fk::Pool &pool) {
    auto mr = new(pool) NModuleReadings<10>();
    for (auto i = 0; i < mr->size(); i++) {
        mr->set(i, (float)fk_random_i32(20, 100));
    }
    return mr;
}
