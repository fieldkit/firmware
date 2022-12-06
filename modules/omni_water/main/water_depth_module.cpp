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

    /**
     * Hello traveller!
     *
     * As of today, we do this manually rather than relying on the
     * self-identification features of these chips. According to the
     * documentation the `version` portion of the first PROM register is
     * supposed to identify the model for these.
     *
     * Only, there seems to be some issues.
     *
     * None of the 30BA sensors I've received or seen in the wild declare
     * themselves properly. In fact, all of the versions come back as 0,
     * indicating that they are 02BA models. Only, we know they aren't because
     * the values produced by them only make sense if you force the model to
     * 30BA.
     *
     * This means we'll need some user intervention to understand which model is
     * actually attached.
     */
    ms5837.setModel(MS5837::MS5837_02BA);
    ms5837.read();

    auto temperature = ms5837.temperature();
    auto pressure = ms5837.pressure();

    auto mr = new (pool) NModuleReadings<2>();
    mr->set(0, SensorReading{ mc.now(), temperature, temperature });
    mr->set(1, SensorReading{ mc.now(), pressure, pressure });

    return mr;
}

} // namespace fk
