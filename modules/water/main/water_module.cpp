#include "water_module.h"
#include "platform.h"
#include "modules/eeprom.h"

namespace fk {

FK_DECLARE_LOGGER("water");

ModuleReturn WaterModule::initialize(ModuleContext mc, Pool &pool) {
    // TODO Not a fan of this, move to ctor?
    FK_ASSERT(pool_ == nullptr);

    pool_ = &pool;

    load_configuration(mc, pool);

    return { ModuleStatus::Ok };
}

bool WaterModule::load_configuration(ModuleContext mc, Pool &pool) {
    return true;
}

ModuleReturn WaterModule::api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
    return { ModuleStatus::Ok };
}

ModuleReturn WaterModule::service(ModuleContext mc, Pool &pool) {
    return { ModuleStatus::Ok };
}

ModuleSensors const *WaterModule::get_sensors(Pool &pool) {
    auto voltage = pool.malloc_with<SensorMetadata>({
        .name = "v",
        .unitOfMeasure = "V",
        .flags = 0,
    });
    return pool.malloc_with<ModuleSensors>({
        .nsensors = 1,
        .sensors = voltage,
    });
}

const char *WaterModule::get_display_name_key() {
    return "modules.water.unknown";
}

ModuleConfiguration const WaterModule::get_configuration(Pool &pool) {
    return { get_display_name_key(), ModulePower::ReadingsOnly, 0, cfg_message_, ModuleOrderProvidesCalibration };
}

ModuleReadings *WaterModule::take_readings(ReadingsContext mc, Pool &pool) {
    logerror("no readings");
    return nullptr;
}

} // namespace fk
