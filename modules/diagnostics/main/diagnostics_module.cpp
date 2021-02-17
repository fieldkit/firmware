#include "diagnostics_module.h"
#include "platform.h"
#include "state.h"
#include "state_ref.h"
#include "temperature.h"

namespace fk {

static SensorMetadata const fk_module_sensor_metas[] = {
    { .name = "battery_charge",  .unitOfMeasure = "%",     .flags = 0 },

    { .name = "battery_vbus",    .unitOfMeasure = "v",     .flags = 0 },
    { .name = "battery_vs",      .unitOfMeasure = "mv",    .flags = 0 },
    { .name = "battery_ma",      .unitOfMeasure = "ma",    .flags = 0 },
    { .name = "battery_power",   .unitOfMeasure = "mw",    .flags = 0 },

    { .name = "solar_vbus",      .unitOfMeasure = "v",     .flags = 0 },
    { .name = "solar_vs",        .unitOfMeasure = "mv",    .flags = 0 },
    { .name = "solar_ma",        .unitOfMeasure = "ma",    .flags = 0 },
    { .name = "solar_power",     .unitOfMeasure = "mw",    .flags = 0 },

    { .name = "free_memory",     .unitOfMeasure = "bytes", .flags = 0 },
    { .name = "uptime",          .unitOfMeasure = "ms",    .flags = 0 },
    { .name = "temperature",     .unitOfMeasure = "C",     .flags = 0 },
};

static ModuleSensors const fk_module_sensors = {
    .nsensors = sizeof(fk_module_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_sensor_metas,
};

ModuleReturn DiagnosticsModule::initialize(ModuleContext mc, Pool &pool) {
    return { ModuleStatus::Ok };
}

ModuleReturn DiagnosticsModule::api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
    connection->busy(0, "unsupported");

    return { ModuleStatus::Fatal };
}

ModuleReturn DiagnosticsModule::service(ModuleContext mc, Pool &pool) {
    return { ModuleStatus::Ok };
}

ModuleSensors const *DiagnosticsModule::get_sensors(Pool &pool) {
    return &fk_module_sensors;
}

ModuleConfiguration DiagnosticsModule::get_configuration(Pool &pool) {
    return ModuleConfiguration{ "modules.diagnostics", ModuleOrderInternal };
}

ModuleReadings *DiagnosticsModule::take_readings(ReadingsContext mc, Pool &pool) {
    CoreTemperature core_temperature_sensor{ get_board()->i2c_core() };

    auto gs = get_global_state_ro();

    auto spmi = fk_standard_page_meminfo();

    auto i = 0u;
    auto mr = new(pool) NModuleReadings<12>();
    mr->set(i++, gs.get()->power.charge);

    mr->set(i++, gs.get()->power.battery.bus_voltage);
    mr->set(i++, gs.get()->power.battery.shunted_voltage);
    mr->set(i++, gs.get()->power.battery.ma);
    mr->set(i++, gs.get()->power.battery.mw);

    mr->set(i++, gs.get()->power.solar.bus_voltage);
    mr->set(i++, gs.get()->power.solar.shunted_voltage);
    mr->set(i++, gs.get()->power.solar.ma);
    mr->set(i++, gs.get()->power.solar.mw);

    mr->set(i++, fk_free_memory() + (spmi.free * StandardPageSize));
    mr->set(i++, fk_uptime());

    float core_temperature = 0.0f;
    if (core_temperature_sensor.read(&core_temperature)) {
        mr->set(i++, core_temperature);
    }

    return mr;
}

} // namespace fk
