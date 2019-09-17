#include "diagnostics_module.h"
#include "platform.h"
#include "state.h"

using namespace fk;

static SensorMetadata const fk_module_sensor_metas[] = {
    { .name = "battery charge",  .unitOfMeasure = "%",     .flags = 0 },
    { .name = "battery voltage", .unitOfMeasure = "v",     .flags = 0 },
    { .name = "free memory",     .unitOfMeasure = "bytes", .flags = 0 },
    { .name = "uptime",          .unitOfMeasure = "ms",    .flags = 0 },
};

static ModuleSensors const fk_module_sensors = {
    .nsensors = sizeof(fk_module_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_sensor_metas,
};

bool DiagnosticsModule::initialize(ModuleContext mc, fk::Pool &pool) {
    return true;
}

ModuleSensors const *DiagnosticsModule::get_sensors(Pool &pool) {
    return &fk_module_sensors;
}

ModuleReadings *DiagnosticsModule::take_readings(ModuleContext mc, fk::Pool &pool) {
    auto mr = new(pool) NModuleReadings<4>();
    mr->set(0, mc.gs()->power.charge);
    mr->set(1, mc.gs()->power.voltage);
    mr->set(2, fk_free_memory());
    mr->set(3, fk_uptime());
    return mr;
}
