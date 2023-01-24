#include "diagnostics_module.h"
#include "platform.h"
#include "state.h"
#include "state_ref.h"
#include "temperature.h"

namespace fk {

static SensorMetadata const fk_module_sensor_metas[] = {
    { .name = "battery_charge", .unitOfMeasure = "%", .uncalibratedUnitOfMeasure = "%", .flags = 0 },

    { .name = "battery_vbus", .unitOfMeasure = "V", .uncalibratedUnitOfMeasure = "V", .flags = 0 },
    { .name = "battery_vs", .unitOfMeasure = "mV", .uncalibratedUnitOfMeasure = "mV", .flags = 0 },
    { .name = "battery_ma", .unitOfMeasure = "mA", .uncalibratedUnitOfMeasure = "mA", .flags = 0 },
    { .name = "battery_power", .unitOfMeasure = "mW", .uncalibratedUnitOfMeasure = "mW", .flags = 0 },

    { .name = "solar_vbus", .unitOfMeasure = "V", .uncalibratedUnitOfMeasure = "V", .flags = 0 },
    { .name = "solar_vs", .unitOfMeasure = "mV", .uncalibratedUnitOfMeasure = "mV", .flags = 0 },
    { .name = "solar_ma", .unitOfMeasure = "mA", .uncalibratedUnitOfMeasure = "mA", .flags = 0 },
    { .name = "solar_power", .unitOfMeasure = "mW", .uncalibratedUnitOfMeasure = "mW", .flags = 0 },

    { .name = "free_memory", .unitOfMeasure = "bytes", .uncalibratedUnitOfMeasure = "bytes", .flags = 0 },
    { .name = "uptime", .unitOfMeasure = "ms", .uncalibratedUnitOfMeasure = "ms", .flags = 0 },
    { .name = "temperature", .unitOfMeasure = "°C", .uncalibratedUnitOfMeasure = "°C", .flags = 0 },
};

static ModuleSensors const fk_module_sensors = {
    .nsensors = sizeof(fk_module_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_sensor_metas,
};

DiagnosticsModule::DiagnosticsModule(Pool &pool) {
}

DiagnosticsModule::~DiagnosticsModule() {
}

ModuleReturn DiagnosticsModule::initialize(ModuleContext mc, Pool &pool) {
    return { ModuleStatus::Ok };
}

ModuleReturn DiagnosticsModule::api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
    connection->busy(0, "unsupported", pool);

    return { ModuleStatus::Fatal };
}

ModuleReturn DiagnosticsModule::service(ModuleContext mc, Pool &pool) {
    return { ModuleStatus::Ok };
}

ModuleSensors const *DiagnosticsModule::get_sensors(Pool &pool) {
    return &fk_module_sensors;
}

ModuleConfiguration const DiagnosticsModule::get_configuration(Pool &pool) {
    return ModuleConfiguration{ "modules.diagnostics", ModuleOrderInternal };
}

ModuleReadings *DiagnosticsModule::take_readings(ReadingsContext mc, Pool &pool) {
    CoreTemperature core_temperature_sensor;

    auto gs = get_global_state_ro();
    auto spmi = fk_standard_page_meminfo();
    auto now = mc.now();

    auto i = 0u;
    auto mr = new (pool) NModuleReadings<12>();
    mr->set(i++, SensorReading{ now, gs.get()->power.charge });

    mr->set(i++, SensorReading{ now, gs.get()->power.battery.bus_voltage });
    mr->set(i++, SensorReading{ now, gs.get()->power.battery.shunted_voltage });
    mr->set(i++, SensorReading{ now, gs.get()->power.battery.ma });
    mr->set(i++, SensorReading{ now, gs.get()->power.battery.mw });

    mr->set(i++, SensorReading{ now, gs.get()->power.solar.bus_voltage });
    mr->set(i++, SensorReading{ now, gs.get()->power.solar.shunted_voltage });
    mr->set(i++, SensorReading{ now, gs.get()->power.solar.ma });
    mr->set(i++, SensorReading{ now, gs.get()->power.solar.mw });

    mr->set(i++, SensorReading{ now, (float)(fk_free_memory() + (spmi.free * StandardPageSize)) });
    mr->set(i++, SensorReading{ now, (float)fk_uptime() });

    float core_temperature = 0.0f;
    if (core_temperature_sensor.read(&core_temperature)) {
        mr->set(i++, SensorReading{ now, core_temperature });
    }

    return mr;
}

} // namespace fk
