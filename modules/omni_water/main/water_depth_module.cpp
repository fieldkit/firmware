#include "water_depth_module.h"

#include "platform.h"
#include "state_ref.h"

#include "MS5837.h"

namespace fk {

FK_DECLARE_LOGGER("waterdepth");

WaterDepthModule::WaterDepthModule(Pool &pool) : pool_(pool.subpool("waterdepth", MaximumConfigurationSize)) {
}

WaterDepthModule::~WaterDepthModule() {
}

ModuleReturn WaterDepthModule::api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
    return { ModuleStatus::Fatal };
}

ModuleReturn WaterDepthModule::service(ModuleContext mc, Pool &pool) {
    return { ModuleStatus::Ok };
}

static SensorMetadata const fk_module_water_depth_sensor_metas[] = {
    { .name = "temp", .unitOfMeasure = "°C", .flags = 0 },
    { .name = "depth", .unitOfMeasure = "pa", .flags = 0 },
};

static ModuleSensors fk_module_water_depth_sensors = {
    .nsensors = sizeof(fk_module_water_depth_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_water_depth_sensor_metas,
};

ModuleSensors const *WaterDepthModule::get_sensors(Pool &pool) {
    return &fk_module_water_depth_sensors;
}

ModuleConfiguration const WaterDepthModule::get_configuration(Pool &pool) {
    return ModuleConfiguration{ "modules.water.depth", ModulePower::ReadingsOnly, nullptr, DefaultModuleOrder };
}

ModuleReturn WaterDepthModule::initialize(ModuleContext mc, Pool &pool) {
    loginfo("initialize");

    return { ModuleStatus::Ok };
}

ModuleReadings *WaterDepthModule::take_readings(ReadingsContext mc, Pool &pool) {
    MS5837 ms5837;

    // TODO Refactor to use our I2C abstraction? Or simply map to Arduino type.
    if (!ms5837.begin(Wire2)) {
        logerror("ms5837:missing");
        return nullptr;
    }

    ms5837.read();

    auto temperature = ms5837.temperature();
    auto pressure = ms5837.pressure(MS5837::Pa);

    auto mr = new (pool) NModuleReadings<2>();
    mr->set(0, SensorReading{ mc.now(), temperature });
    mr->set(1, SensorReading{ mc.now(), pressure });

    return mr;
}

} // namespace fk
