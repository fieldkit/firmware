#include "atlas_module.h"
#include "atlas_api.h"
#include "curves.h"
#include "modules/eeprom.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("atlas");

AtlasModule::AtlasModule(Pool &pool) : pool_(pool.subpool("atlas", MaximumConfigurationSize)) {
}

AtlasModule::~AtlasModule() {
}

ModuleReturn AtlasModule::initialize(ModuleContext mc, Pool &pool) {
    load_configuration(mc, pool);

    auto atlas = OemAtlas{ mc.module_bus() };
    if (!atlas.find()) {
        logerror("no atlas module (ms::fatal)");
        return { ModuleStatus::Fatal };
    }

    type_ = atlas.type();
    address_ = atlas.address();

    return { ModuleStatus::Ok };
}

bool AtlasModule::load_configuration(ModuleContext mc, Pool &pool) {
    ModuleEeprom eeprom{ mc.module_bus() };

    cfg_ = { nullptr, nullptr };
    pool_->clear();

    size_t size = 0;
    uint8_t *buffer = nullptr;
    if (!eeprom.read_configuration(&buffer, size, &pool)) {
        logwarn("error reading configuration");
    } else if (size > 0) {
        auto cfg = fk_module_configuration_decoding_new(pool_);
        auto stream = pb_istream_from_buffer(buffer, size);
        if (!pb_decode_delimited(&stream, fk_data_ModuleConfiguration_fields, cfg)) {
            logerror("mod-cfg: error decoding ");
            return false;
        } else {
            loginfo("mod-cfg: decoded");
            cfg_ = { pool_->wrap_copy(buffer, size), cfg };
        }
    }

    return true;
}

ModuleReturn AtlasModule::api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
    if (type_ == AtlasSensorType::Unknown) {
        if (!initialize(mc, pool)) {
            logerror("error initializing (ms::fatal)");
            return { ModuleStatus::Fatal };
        }
    }

    OemAtlas atlas{ mc.module_bus(), address_, type_ };
    AtlasApi api{ type_, atlas };
    if (!api.handle(mc, connection, pool)) {
        logerror("error handling api (ms::fatal)");
        return { ModuleStatus::Fatal };
    }

    load_configuration(mc, pool);

    return { ModuleStatus::Ok };
}

ModuleReturn AtlasModule::service(ModuleContext mc, Pool &pool) {
    return { ModuleStatus::Ok };
}

ModuleSensors const *AtlasModule::get_sensors(Pool &pool) {
    switch (type_) {
    case AtlasSensorType::Ec: {
        auto meta = pool.malloc_with<SensorMetadata, 3>({ {
                                                              .name = "ec",
                                                              .unitOfMeasure = "µS/cm",
                                                              .uncalibratedUnitOfMeasure = "µS/cm",
                                                              .flags = 0,
                                                          },
                                                          {
                                                              .name = "tds",
                                                              .unitOfMeasure = "ppm",
                                                              .uncalibratedUnitOfMeasure = "ppm",
                                                              .flags = 0,
                                                          },
                                                          {
                                                              .name = "salinity",
                                                              .unitOfMeasure = "",
                                                              .uncalibratedUnitOfMeasure = "",
                                                              .flags = 0,
                                                          } });
        return pool.malloc_with<ModuleSensors>({
            .nsensors = 3,
            .sensors = meta,
        });
    }
    case AtlasSensorType::Ph: {
        auto meta = pool.malloc_with<SensorMetadata>({
            .name = "ph",
            .unitOfMeasure = "pH",
            .uncalibratedUnitOfMeasure = "pH",
            .flags = 0,
        });
        return pool.malloc_with<ModuleSensors>({
            .nsensors = 1,
            .sensors = meta,
        });
    }
    case AtlasSensorType::Do: {
        auto meta = pool.malloc_with<SensorMetadata>({
            .name = "dox",
            .unitOfMeasure = "mg/L",
            .uncalibratedUnitOfMeasure = "mg/L",
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
            .unitOfMeasure = "°C",
            .uncalibratedUnitOfMeasure = "°C",
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
            .uncalibratedUnitOfMeasure = "mV",
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

const char *AtlasModule::get_display_name_key() {
    switch (type_) {
    case AtlasSensorType::Ec:
        return "modules.water.ec";
    case AtlasSensorType::Ph:
        return "modules.water.ph";
    case AtlasSensorType::Do:
        return "modules.water.dox";
    case AtlasSensorType::Temp:
        return "modules.water.temp";
    case AtlasSensorType::Orp:
        return "modules.water.orp";
    default:
        return "modules.water.unknown";
    }
}

ModuleConfiguration const AtlasModule::get_configuration(Pool &pool) {
    // Make sure temperature is serviced before any of the other water modules.
    switch (type_) {
    case AtlasSensorType::Temp:
        return ModuleConfiguration{ get_display_name_key(), ModulePower::ReadingsOnly, cfg_.first, ModuleOrderProvidesCalibration };
    default:
        return ModuleConfiguration{ get_display_name_key(), ModulePower::ReadingsOnly, cfg_.first, DefaultModuleOrder };
    }
}

ModuleReadings *AtlasModule::take_readings(ReadingsContext mc, Pool &pool) {
    if (type_ == AtlasSensorType::Unknown) {
        return nullptr;
    }

    auto default_curve = create_noop_curve(pool);
    auto curve = create_curve(default_curve, (uint32_t)type_, cfg_.second, pool);

    auto atlas = OemAtlas{ mc.module_bus(), address_, type_ };
    if (!atlas.wake()) {
        logerror("wake failed (take-readings)");
        return nullptr;
    }

    fk_delay(1000);

    auto temperature = get_temperature(mc);
    auto salinity = get_salinity(mc);
    auto pressure = get_pressure(mc);
    auto compensations = Compensation{
        .temperature = temperature,
        .salinity = salinity,
        .pressure = pressure,
    };
    if (!atlas.compensate(compensations)) {
        logerror("compensate failed");
    }

    size_t number_of_values = 0;
    float values[ATLAS_MAXIMUM_VALUES];
    for (auto i = 0u; i < 3u; ++i) {
        number_of_values = 0u;

        if (!atlas.read(values, number_of_values)) {
            atlas.leds(true);
            atlas.hibernate();
            logerror("readings failed");
            return nullptr;
        }

        /* We're chasing down an issue with the RTD sensor, so we're
         * going to retry if we end up with a known bad value. In this
         * situation the sensor is returning -1023.0 and so we're
         * seeing if trying multiple reads helps the situation.
         */
        if (number_of_values == 1) {
            if (values[0] > -1000) {
                break;
            }
        }
    }

    if (!atlas.leds(true)) {
        logerror("leds failed");
        return nullptr;
    }

    if (!atlas.hibernate()) {
        logerror("hibernate failed");
        return nullptr;
    }

    auto mr = new (pool) NModuleReadings<ATLAS_MAXIMUM_VALUES>(number_of_values);
    for (auto i = 0u; i < mr->size(); ++i) {
        if (i == 0) {
            mr->set(i, SensorReading{ mc.now(), values[i], curve->apply(values[i]) });
        } else {
            mr->set(i, SensorReading{ mc.now(), values[i] });
        }
    }

    return mr;
}

optional<float> AtlasModule::get_temperature(ReadingsContext mc) {
    return nullopt;
}

optional<float> AtlasModule::get_salinity(ReadingsContext mc) {
    return nullopt;
}

optional<float> AtlasModule::get_pressure(ReadingsContext mc) {
    return nullopt;
}

} // namespace fk
