#include "water_module.h"
#include "platform.h"
#include "atlas_api.h"
#include "modules/eeprom.h"

namespace fk {

FK_DECLARE_LOGGER("water");

class Curve {
public:
    virtual float apply(float uncalibrated) = 0;
};

static Curve *create_curve(fk_data_ModuleConfiguration *cfg, Pool &pool);

ModuleReturn WaterModule::initialize(ModuleContext mc, Pool &pool) {
    // TODO Not a fan of this, move to ctor?
    FK_ASSERT(pool_ == nullptr);

    pool_ = &pool;

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

bool WaterModule::load_configuration(ModuleContext mc, Pool &pool) {
    ModuleEeprom eeprom{ mc.module_bus() };

    cfg_message_ = nullptr;
    cfg_ = nullptr;

    size_t size = 0;
    auto buffer = (uint8_t *)pool.malloc(MaximumConfigurationSize);
    bzero(buffer, MaximumConfigurationSize);
    if (!eeprom.read_configuration(buffer, size, MaximumConfigurationSize)) {
        logwarn("error reading configuration");
    } else if (size > 0) {
        auto cfg = fk_module_configuration_decoding_new(pool_);
        auto stream = pb_istream_from_buffer(buffer, size);
        if (!pb_decode_delimited(&stream, fk_data_ModuleConfiguration_fields, cfg)) {
            logerror("mod-cfg: error decoding ");
            return false;
        }
        else {
            loginfo("mod-cfg: decoded");
            cfg_message_ = pool_->wrap_copy(buffer, size);
            cfg_ = cfg;
        }
    }

    return true;
}

ModuleReturn WaterModule::api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
    if (type_ == AtlasSensorType::Unknown) {
        if (!initialize(mc, pool)) {
            logerror("error initializing (ms::fatal)");
            return { ModuleStatus::Fatal };
        }
    }

    OemAtlas atlas{ mc.module_bus(), address_, type_  };
    AtlasApi api{ type_, atlas };
    if (!api.handle(mc, connection, pool)) {
        logerror("error handling api (ms::fatal)");
        return { ModuleStatus::Fatal };
    }

    load_configuration(mc, pool);

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
            .unitOfMeasure = "pH",
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
    case AtlasSensorType::Do: return "modules.water.dox";
    case AtlasSensorType::Temp: return "modules.water.temp";
    case AtlasSensorType::Orp: return "modules.water.orp";
    default: return "modules.water.unknown";
    }
}

ModuleConfiguration const WaterModule::get_configuration(Pool &pool) {
    // Make sure temperature is serviced before any of the other water modules.
    switch (type_) {
    case AtlasSensorType::Temp:
        return { get_display_name_key(), ModulePower::ReadingsOnly, 0, cfg_message_, ModuleOrderProvidesCalibration };
    default:
        return { get_display_name_key(), ModulePower::ReadingsOnly, 0, cfg_message_, DefaultModuleOrder };
    }
}

ModuleReadings *WaterModule::take_readings(ReadingsContext mc, Pool &pool) {
    if (type_ == AtlasSensorType::Unknown) {
        return nullptr;
    }

    auto curve = create_curve(cfg_, pool);

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

    // fk_delay(1000);

    if (!atlas.hibernate()) {
        logerror("hibernate failed");
        return nullptr;
    }

    auto mr = new(pool) NModuleReadings<ATLAS_MAXIMUM_VALUES>(number_of_values);
    if (mr->size() == 1) {
        mr->set(0, ModuleReading{ values[0], curve->apply(values[0]) });
    }
    else {
        for (auto i = 0u; i < mr->size(); ++i) {
            mr->set(i, ModuleReading{ values[i] });
        }
    }

    return mr;
}

optional<float> WaterModule::get_temperature(ReadingsContext mc) {
    for (auto &r : mc.readings()) {
        if (r.meta->manufacturer == FK_MODULES_MANUFACTURER && r.meta->kind == FK_MODULES_KIND_WATER_TEMP) {
            if (r.readings->size() == 1) {
                return { r.readings->get(0).calibrated };
            }
        }
    }
    return nullopt;
}

optional<float> WaterModule::get_salinity(ReadingsContext mc) {
    for (auto &r : mc.readings()) {
        if (r.meta->manufacturer == FK_MODULES_MANUFACTURER && r.meta->kind == FK_MODULES_KIND_WATER_EC) {
            if (r.readings->size() == 3) {
                return { r.readings->get(2).calibrated };
            }
        }
    }
    return nullopt;
}

optional<float> WaterModule::get_pressure(ReadingsContext mc) {
    return nullopt;
}

class NoopCurve : public Curve {
public:
    virtual float apply(float uncalibrated) override {
        return uncalibrated;
    }
};

class LinearCurve : public Curve {
private:
    float b_{ 0.0f };
    float m_{ 1.0f };

public:
    LinearCurve(fk_data_ModuleConfiguration *cfg) {
        if (cfg == nullptr) return;
        if (!cfg->has_calibration) return;
        if (!cfg->calibration.has_coefficients) return;
        if (cfg->calibration.coefficients.values.arg == nullptr) return;

        auto values_array = reinterpret_cast<pb_array_t *>(cfg->calibration.coefficients.values.arg);
        if (values_array->length != 2) return;

        auto values = reinterpret_cast<float*>(values_array->buffer);
        b_ = values[0];
        m_ = values[1];

        loginfo("cal: b = %f m = %f", b_, m_);
    }

public:
    virtual float apply(float uncalibrated) override {
        return b_ + (uncalibrated * m_);
    }
};

static Curve *create_curve(fk_data_ModuleConfiguration *cfg, Pool &pool) {
    if (cfg == nullptr) return new (pool) NoopCurve();
    if (cfg->calibration.type == fk_data_CurveType_CURVE_LINEAR) return new (pool) LinearCurve(cfg);
    return new (pool) NoopCurve();
}

} // namespace fk
