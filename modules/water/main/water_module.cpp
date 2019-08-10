#include "water_module.h"
#include "platform.h"
#include "oem_atlas.h"

using namespace fk;

FK_DECLARE_LOGGER("water");

bool WaterModule::initialize(ModuleContext mc, fk::Pool &pool) {
    loginfo("initialize");
    return true;
}

ModuleReadings *WaterModule::take_readings(ModuleContext mc, fk::Pool &pool) {
    auto atlas = OemAtlas{ mc.module_bus() };

    if (!atlas.find(PH_DEFAULT_ADDRESS)) {
        return nullptr;
    }

    if (!atlas.wake()) {
        return nullptr;
    }

    size_t number_of_values = 0;
    float values[ATLAS_MAXIMUM_VALUES];
    if (!atlas.read(values, number_of_values)) {
        return nullptr;
    }

    if (!atlas.leds(false)) {
        return nullptr;
    }

    if (!atlas.hibernate()) {
        return nullptr;
    }

    auto mr = new(pool) NModuleReadings<ATLAS_MAXIMUM_VALUES>(number_of_values);
    for (auto i = 0; i < mr->size(); ++i) {
        loginfo("atlas(%s) -> (value = %f)", atlas.name(), values[i]);
        mr->set(i, values[i]);
    }

    return mr;
}
