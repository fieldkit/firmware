#include "water_module.h"
#include "platform.h"
#include "atlas_api.h"

namespace fk {

FK_DECLARE_LOGGER("water");

ModuleReturn WaterModule::initialize(ModuleContext mc, Pool &pool) {
    auto atlas = OemAtlas{ mc.module_bus() };
    if (!atlas.find()) {
        return { ModuleStatus::Fatal };
    }

    type_ = atlas.type();
    address_ = atlas.address();

    return { ModuleStatus::Ok };
}

ModuleReturn WaterModule::api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
    if (type_ == AtlasSensorType::Unknown) {
        if (!initialize(mc, pool)) {
            return { ModuleStatus::Fatal };
        }
    }

    OemAtlas atlas{ mc.module_bus(), address_, type_  };
    AtlasApi api{ type_, atlas };
    if (!api.handle(connection, pool)) {
        return { ModuleStatus::Fatal };
    }

    return { ModuleStatus::Ok };
}

ModuleReturn WaterModule::service(ModuleContext mc, Pool &pool) {
    return { ModuleStatus::Ok };
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

const char *WaterModule::get_display_name_key() {
    switch (type_) {
    case AtlasSensorType::Ec: return "modules.water.ec";
    case AtlasSensorType::Ph: return "modules.water.ph";
    case AtlasSensorType::Do: return "modules.water.do";
    case AtlasSensorType::Temp: return "modules.water.temp";
    case AtlasSensorType::Orp: return "modules.water.orp";
    default: return "modules.water.unknown";
    }
}

ModuleConfiguration WaterModule::get_configuration(Pool &pool) {
    return { get_display_name_key() };
}

ModuleReadings *WaterModule::take_readings(ModuleContext mc, Pool &pool) {
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

} // namespace fk
