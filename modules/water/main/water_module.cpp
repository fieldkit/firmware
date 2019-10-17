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
    address_ = atlas.address();

    return true;
}

ModuleSensors const *WaterModule::get_sensors(Pool &pool) {
    switch (type_) {
    case AtlasSensorType::Ec: {
        auto meta = pool.malloc_with<SensorMetadata, 3>({
            {
                .name = "ec",
                .unitOfMeasure = "µS/cm",
                .flags = 0,
            },
            {
                .name = "tds",
                .unitOfMeasure = "ppm",
                .flags = 0,
            },
            {
                .name = "salinity",
                .unitOfMeasure = "",
                .flags = 0,
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
            .flags = 0,
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
            .flags = 0,
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
            .flags = 0,
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
            .flags = 0,
        });
        return pool.malloc_with<ModuleSensors>({
            .nsensors = 1,
            .sensors = meta,
        });
    }
    default: {
        break;
    }
    }

    logerror("unknown atlas module!");

    return nullptr;
}

ModuleReadings *WaterModule::take_readings(ModuleContext mc, fk::Pool &pool) {
    if (type_ == AtlasSensorType::Unknown) {
        return nullptr;
    }

    auto atlas = OemAtlas{ mc.module_bus(), address_, type_ };
    if (!atlas.wake()) {
        logerror("wake failed");
        return nullptr;
    }

    size_t number_of_values = 0;
    float values[ATLAS_MAXIMUM_VALUES];
    if (!atlas.read(values, number_of_values)) {
        atlas.leds(true);
        atlas.hibernate();
        logerror("readings failed");
        return nullptr;
    }

    if (!atlas.leds(true)) {
        logerror("leds failed");
        return nullptr;
    }

    if (!atlas.hibernate()) {
        logerror("hibernate failed");
        return nullptr;
    }

    auto mr = new(pool) NModuleReadings<ATLAS_MAXIMUM_VALUES>(number_of_values);
    for (size_t i = 0; i < mr->size(); ++i) {
        mr->set(i, values[i]);
    }

    return mr;
}
