#include "random_module.h"
#include "hal/random.h"

using namespace fk;

bool RandomModule::initialize(ModuleContext mc, fk::Pool &pool) {
    return true;
}

static SensorMetadata const fk_module_random_sensor_metas[] = {
    { .name = "random_0", .unitOfMeasure = "", .flags = 0 },
    { .name = "random_1", .unitOfMeasure = "", .flags = 0 },
    { .name = "random_2", .unitOfMeasure = "", .flags = 0 },
    { .name = "random_3", .unitOfMeasure = "", .flags = 0 },
    { .name = "random_4", .unitOfMeasure = "", .flags = 0 },
    { .name = "random_5", .unitOfMeasure = "", .flags = 0 },
    { .name = "random_6", .unitOfMeasure = "", .flags = 0 },
    { .name = "random_7", .unitOfMeasure = "", .flags = 0 },
    { .name = "random_8", .unitOfMeasure = "", .flags = 0 },
    { .name = "random_9", .unitOfMeasure = "", .flags = 0 },
};

static ModuleSensors fk_module_random_sensors = {
    .nsensors = sizeof(fk_module_random_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_random_sensor_metas,
};

ModuleSensors const *RandomModule::get_sensors(Pool &pool) {
    return &fk_module_random_sensors;
}

ModuleReadings *RandomModule::take_readings(ModuleContext mc, fk::Pool &pool) {
    #if defined(FK_MODULE_RANDOM_FIXED)
    auto nreadings = 10;
    #else
    auto nreadings = fk_random_i32(0, 10);
    #endif
    auto mr = new(pool) NModuleReadings<10>(nreadings);
    for (auto i = 0; i < nreadings; i++) {
        mr->set(i, (float)fk_random_i32(20, 100));
    }
    return mr;
}
