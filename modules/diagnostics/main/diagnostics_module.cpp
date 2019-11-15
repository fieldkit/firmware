#include "diagnostics_module.h"
#include "platform.h"
#include "state.h"
#include "temperature.h"

using namespace fk;

static SensorMetadata const fk_module_sensor_metas[] = {
    { .name = "battery_charge",  .unitOfMeasure = "%",     .flags = 0 },
    { .name = "battery_voltage", .unitOfMeasure = "v",     .flags = 0 },
    { .name = "free_memory",     .unitOfMeasure = "bytes", .flags = 0 },
    { .name = "uptime",          .unitOfMeasure = "ms",    .flags = 0 },
    { .name = "temperature",     .unitOfMeasure = "C",     .flags = 0 },
};

static ModuleSensors const fk_module_sensors = {
    .nsensors = sizeof(fk_module_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_sensor_metas,
};

bool DiagnosticsModule::initialize(ModuleContext mc, Pool &pool) {
    return true;
}

bool DiagnosticsModule::service(ModuleContext mc, Pool &pool) {
    return true;
}

ModuleSensors const *DiagnosticsModule::get_sensors(Pool &pool) {
    return &fk_module_sensors;
}

ModuleConfiguration DiagnosticsModule::get_configuration(Pool &pool) {
    return { };
}

ModuleReadings *DiagnosticsModule::take_readings(ModuleContext mc, Pool &pool) {
    CoreTemperature core_temperature_sensor{ get_board()->i2c_core() };

    auto mr = new(pool) NModuleReadings<5>();
    mr->set(0, mc.gs()->power.charge);
    mr->set(1, mc.gs()->power.voltage);
    mr->set(2, fk_free_memory());
    mr->set(3, fk_uptime());

    float core_temperature = 0.0f;
    if (core_temperature_sensor.read(&core_temperature)) {
        mr->set(4, core_temperature);
    }

    return mr;
}
