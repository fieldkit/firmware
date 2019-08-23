#include "water_module.h"
#include "platform.h"

using namespace fk;

FK_DECLARE_LOGGER("water");

bool WaterModule::initialize(ModuleContext mc, fk::Pool &pool) {
    auto atlas = OemAtlas{ mc.module_bus() };
    if (!atlas.find()) {
        return false;
    }

    type_ = atlas.type();

    return true;
}

ModuleSensors const *WaterModule::get_sensors(ModuleContext mc, Pool &pool) {
    switch (type_) {
    case AtlasSensorType::Ec: {
        auto meta = pool.malloc_with<SensorMetadata, 3>({
            {
                .name = "ec",
                .unitOfMeasure = "µS/cm",
            },
            {
                .name = "tds",
                .unitOfMeasure = "ppm",
            },
            {
                .name = "salinity",
                .unitOfMeasure = "",
            }
        });
        return pool.malloc_with<ModuleSensors>({
            .nsensors = 3,
            .sensors = meta,
        });
    }
    case AtlasSensorType::Ph: {
        auto meta = pool.malloc_with<SensorMetadata>({
            .name = "ph",
            .unitOfMeasure = "",
        });
        return pool.malloc_with<ModuleSensors>({
            .nsensors = 1,
            .sensors = meta,
        });
    }
    case AtlasSensorType::Do: {
        auto meta = pool.malloc_with<SensorMetadata>({
            .name = "do",
            .unitOfMeasure = "mg/L",
        });
        return pool.malloc_with<ModuleSensors>({
            .nsensors = 1,
            .sensors = meta,
        });
    }
    case AtlasSensorType::Temp: {
        auto meta = pool.malloc_with<SensorMetadata>({
            .name = "temp",
            .unitOfMeasure = "C",
        });
        return pool.malloc_with<ModuleSensors>({
            .nsensors = 1,
            .sensors = meta,
        });
    }
    case AtlasSensorType::Orp: {
        auto meta = pool.malloc_with<SensorMetadata>({
            .name = "orp",
            .unitOfMeasure = "mV",
        });
        return pool.malloc_with<ModuleSensors>({
            .nsensors = 1,
            .sensors = meta,
        });
    }
    }

    logerror("unknown atlas module!");

    FK_ASSERT(false);

    return nullptr;
}

ModuleReadings *WaterModule::take_readings(ModuleContext mc, fk::Pool &pool) {
    FK_ASSERT(type_ != AtlasSensorType::Unknown);

    auto atlas = OemAtlas{ mc.module_bus() };
    if (!atlas.find()) {
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
