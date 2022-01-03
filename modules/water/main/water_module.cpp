#include "water_module.h"
#include "modules/eeprom.h"
#include "platform.h"
#include "state_ref.h"
#include "water_api.h"
#include "mpl3115a2.h"

namespace fk {

FK_DECLARE_LOGGER("water");

#define FK_MCP2803_ADDRESS 0x22
#define FK_ADS1219_ADDRESS 0x45

#define FK_MCP2803_IODIR 0b00000010
#define FK_MCP2803_GPPU  0b00000010

#define FK_MCP2803_GPIO_ON  0b00000001
#define FK_MCP2803_GPIO_OFF 0b00000000

#define FK_MCP2803_GPIO_EXCITE_ON  0b00000101
#define FK_MCP2803_GPIO_EXCITE_OFF 0b00000001

class NoopReadyChecker : public Ads1219ReadyChecker {
public:
    bool block_until_ready(TwoWireWrapper &bus) override {
        return true;
    }
};

class Ads1219ReadyAfterDelay : public Ads1219ReadyChecker {
private:
    uint32_t delay_{ 1 };

public:
    Ads1219ReadyAfterDelay() {
    }

public:
    bool block_until_ready(TwoWireWrapper &bus) override {
        fk_delay(delay_);
        return true;
    }
};

class UnexciteBeforeReadyChecker : public Ads1219ReadyChecker {
private:
    Mcp2803 &mcp2803_;
    bool exciting_{ false };

public:
    UnexciteBeforeReadyChecker(Mcp2803 &mcp2803, bool exciting) : mcp2803_(mcp2803), exciting_(exciting) {
    }

public:
    bool block_until_ready(TwoWireWrapper &bus) override {
        if (!mcp2803_.configure(FK_MCP2803_IODIR, FK_MCP2803_GPPU, FK_MCP2803_GPIO_EXCITE_OFF)) {
            logerror("mcp2803::configure-excite");
            return false;
        }
        return true;
    }
};

class Mcp2803ReadyChecker : public Ads1219ReadyChecker {
private:
    Mcp2803 &mcp2803_;

public:
    Mcp2803ReadyChecker(Mcp2803 &mcp2803) : mcp2803_(mcp2803) {
    }

public:
    bool block_until_ready(TwoWireWrapper &bus) override {
        auto give_up = fk_uptime() + 1000;
        while (fk_uptime() < give_up) {
            uint8_t gpio{ 0 };

            if (!mcp2803_.read_gpio(gpio)) {
                return false;
            }

            logdebug("gpio: 0x%x", gpio);

            if (!(gpio & 0x2)) {
                return true;
            }

            fk_delay(20);
        }
        return false;
    }
};

WaterModule::WaterModule(Pool &pool) : pool_(pool.subpool("water", MaximumConfigurationSize)) {
}

WaterModule::~WaterModule() {
}

ModuleReturn WaterModule::initialize(ModuleContext mc, Pool &pool) {
    loginfo("initialize");

    if (!load_configuration(mc, pool)) {
        return { ModuleStatus::Fatal };
    }

    auto &bus = mc.module_bus();

    Mcp2803 mcp{ bus, FK_MCP2803_ADDRESS };
    NoopReadyChecker unused_ready_checker;
    Ads1219 ads{ bus, FK_ADS1219_ADDRESS, &unused_ready_checker };

    if (!initialize(mcp, ads)) {
        return { ModuleStatus::Fatal };
    }

    return { ModuleStatus::Ok };
}

bool WaterModule::initialize(Mcp2803 &mcp, Ads1219 &ads) {
    if (!mcp.configure(FK_MCP2803_IODIR, FK_MCP2803_GPPU, FK_MCP2803_GPIO_OFF)) {
        logerror("mcp2803::begin");
        return false;
    }

    if (!mcp.configure(FK_MCP2803_IODIR, FK_MCP2803_GPPU, FK_MCP2803_GPIO_ON)) {
        logerror("mcp2803::begin");
        return false;
    }

    fk_delay(100);

    if (!ads.begin()) {
        logerror("ads1219::begin");
        return false;
    }

    if (!ads.configure(Ads1219VoltageReference::Internal, Ads1219Channel::Diff_0_1, Ads1219Gain::One, Ads1219DataRate::DataRate_1000)) {
        logerror("ads1219::configure");
        return false;
    }

    return true;
}

bool WaterModule::load_configuration(ModuleContext mc, Pool &pool) {
    ModuleEeprom eeprom{ mc.module_bus() };

    logdebug("reading header");

    // We need the header to know the kind of module we are so if that
    // fails then we're in pretty bad shape.
    bzero(&header_, sizeof(ModuleHeader));
    if (!eeprom.read_header(header_)) {
        logwarn("error reading header");
        return false;
    }

    if (!fk_module_header_valid(&header_)) {
        logwarn("invalid header");
        return false;
    }

    loginfo("have header: mk=%02" PRIx32 "%02" PRIx32, header_.manufacturer, header_.kind);

    cfg_message_ = nullptr;
    cfg_ = nullptr;
    pool_->clear();

    size_t size = 0;
    auto buffer = (uint8_t *)pool.malloc(MaximumConfigurationSize);
    bzero(buffer, MaximumConfigurationSize);
    if (!eeprom.read_configuration(buffer, size, MaximumConfigurationSize)) {
        logwarn("mod-cfg: reading");
    } else if (size > 0) {
        auto cfg = fk_module_configuration_decoding_new(pool_);
        auto stream = pb_istream_from_buffer(buffer, size);
        if (!pb_decode_delimited(&stream, fk_data_ModuleConfiguration_fields, cfg)) {
            // Some modules consider this an error. We continue along uncalibrated.
            logwarn("mod-cfg: decoding");
        } else {
            loginfo("mod-cfg: decoded");
            cfg_message_ = pool_->wrap_copy(buffer, size);
            cfg_ = cfg;
        }
    }

    return true;
}

ModuleReturn WaterModule::api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
    WaterApi api;
    if (!api.handle(mc, connection, pool)) {
        logerror("api (ms::fatal)");
        return { ModuleStatus::Fatal };
    }

    load_configuration(mc, pool);

    return { ModuleStatus::Ok };
}

ModuleReturn WaterModule::service(ModuleContext mc, Pool &pool) {
    return { ModuleStatus::Ok };
}

static SensorMetadata const fk_module_water_do_sensor_metas[] = {
    { .name = "do", .unitOfMeasure = "%", .flags = 0 },
    { .name = "temperature", .unitOfMeasure = "°C", .flags = FK_MODULES_FLAG_INTERNAL },
    { .name = "pressure", .unitOfMeasure = "kPa", .flags = FK_MODULES_FLAG_INTERNAL },
};

static ModuleSensors fk_module_water_do_sensors = {
    .nsensors = sizeof(fk_module_water_do_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_water_do_sensor_metas,
};

ModuleSensors const *WaterModule::get_sensors(Pool &pool) {
    SensorMetadata *sensors = nullptr;

    switch (header_.kind) {
    case FK_MODULES_KIND_WATER_PH:
        sensors = pool.malloc_with<SensorMetadata>({
            .name = "ph",
            .unitOfMeasure = "pH",
            .flags = 0,
        });
        break;
    case FK_MODULES_KIND_WATER_EC:
        sensors = pool.malloc_with<SensorMetadata>({
            .name = "ec",
            .unitOfMeasure = "µS/cm",
            .flags = 0,
        });
        break;
    case FK_MODULES_KIND_WATER_DO:
        return &fk_module_water_do_sensors;
    case FK_MODULES_KIND_WATER_TEMP:
        sensors = pool.malloc_with<SensorMetadata>({
            .name = "temp",
            .unitOfMeasure = "°C",
            .flags = 0,
        });
        break;
    case FK_MODULES_KIND_WATER_ORP:
        sensors = pool.malloc_with<SensorMetadata>({
            .name = "orp",
            .unitOfMeasure = "mV",
            .flags = 0,
        });
        break;
    default:
        logwarn("unknown water module kind: %d", header_.kind);
        return nullptr;
    };

    return pool.malloc_with<ModuleSensors>({
        .nsensors = 1,
        .sensors = sensors,
    });
}

const char *WaterModule::get_display_name_key() {
    switch (header_.kind) {
    case FK_MODULES_KIND_WATER_PH:
        return "modules.water.ph";
    case FK_MODULES_KIND_WATER_EC:
        return "modules.water.ec";
    case FK_MODULES_KIND_WATER_DO:
        return "modules.water.do";
    case FK_MODULES_KIND_WATER_TEMP:
        return "modules.water.temp";
    case FK_MODULES_KIND_WATER_ORP:
        return "modules.water.orp";
    default:
        return "modules.water.unknown";
    };
}

ModuleConfiguration const WaterModule::get_configuration(Pool &pool) {
    switch (header_.kind) {
    case FK_MODULES_KIND_WATER_TEMP:
        return ModuleConfiguration{ get_display_name_key(), ModulePower::ReadingsOnly, cfg_message_, ModuleOrderProvidesCalibration };
    };
    return ModuleConfiguration{ get_display_name_key(), ModulePower::ReadingsOnly, cfg_message_, DefaultModuleOrder };
}

bool WaterModule::averaging_enabled() {
    switch (header_.kind) {
    case FK_MODULES_KIND_WATER_DO: {
        return true;
    }
    default:
        return false;
    };
}

bool WaterModule::excite_enabled() {
    switch (header_.kind) {
    case FK_MODULES_KIND_WATER_EC: {
        auto gs = get_global_state_ro();
        if (gs.get()->debugging.unexciting) {
            return false;
        }
        return true;
    }
    default:
        return false;
    };
}

/*
 * To avoid confusing users by displaying volts for the units on uncalibrated
 * sensors we apply a default curve to each module. These modules are stable
 * enough that these curves gives a reasonable representation out of the box.
 * They came from extensive testing by the amazing Pete Marchetto with fancy
 * test equipment and should only change when the hardware does or we're better
 * able to define them.
 */
Curve *WaterModule::create_modules_default_curve(Pool &pool) {
    switch (header_.kind) {
    case FK_MODULES_KIND_WATER_TEMP: {
        constexpr float TempDefaultCalibrationB = 610.77;
        constexpr float TempDefaultCalibrationM = -831.84;
        return create_curve(fk_data_CurveType_CURVE_LINEAR, TempDefaultCalibrationB, TempDefaultCalibrationM, pool);
    }
    case FK_MODULES_KIND_WATER_PH: {
        constexpr float PhDefaultCalibrationB = -18.75;
        constexpr float PhDefaultCalibrationM = 15.625;
        return create_curve(fk_data_CurveType_CURVE_LINEAR, PhDefaultCalibrationB, PhDefaultCalibrationM, pool);
    }
    case FK_MODULES_KIND_WATER_DO: {
        constexpr float DoDefaultCalibrationB = 2.8711;
        constexpr float DoDefaultCalibrationM = 3.4211;
        return create_curve(fk_data_CurveType_CURVE_LINEAR, DoDefaultCalibrationB, DoDefaultCalibrationM, pool);
    }
    case FK_MODULES_KIND_WATER_ORP: {
        constexpr float OrpDefaultCalibrationB = 0;
        constexpr float OrpDefaultCalibrationM = 1000;
        return create_curve(fk_data_CurveType_CURVE_LINEAR, OrpDefaultCalibrationB, OrpDefaultCalibrationM, pool);
    }
    case FK_MODULES_KIND_WATER_EC: {
        constexpr float EcDefaultCalibrationA = 1e7;
        constexpr float EcDefaultCalibrationB = -6.683;
        return create_curve(fk_data_CurveType_CURVE_EXPONENTIAL, EcDefaultCalibrationA, EcDefaultCalibrationB, pool);
    }
    default:
        return create_noop_curve(pool);
    };
}

Ads1219ReadyChecker *WaterModule::get_ready_checker(Mcp2803 &mcp, Pool &pool) {
    if (excite_enabled()) {
        return new (pool) UnexciteBeforeReadyChecker{ mcp, true };
    }
    return new (pool) Mcp2803ReadyChecker{ mcp };
}

ModuleReadings *WaterModule::take_readings(ReadingsContext mc, Pool &pool) {
    auto &bus = mc.module_bus();

    Mcp2803 mcp{ bus, FK_MCP2803_ADDRESS };

    auto uptime = fk_uptime();

    // If we were locked out, check to see if it's expired, otherwise we return
    // nothing, no readings. It may be necessary later to actually specify what
    // happened to the caller.
    if (unlocked_ > 0) {
        if (uptime < unlocked_) {
            auto remaining = unlocked_ - uptime;
            if (remaining < 0) {
                loginfo("[%d] locked (negative) %" PRIu32, mc.position(), remaining);
                unlocked_ = fk_uptime() + OneMinuteMs;
                return new (pool) EmptyReadings();
            }
            if (remaining > FiveSecondsMs) {
                loginfo("[%d] locked %" PRIu32, mc.position(), remaining);
                return new (pool) EmptyReadings();
            }

            loginfo("[%d] locked %" PRIu32 " waiting for expiration.", mc.position(), remaining);
            fk_delay(remaining);
        } else {
            loginfo("[%d] locked expired", mc.position());
        }

        unlocked_ = 0;
    } else {
        loginfo("[%d] unlocked", mc.position());
    }

    // TODO We could move the excite logic itself into this and clean up the
    // branch below, gonna hold off until we've tested longer, though.
    auto checker = get_ready_checker(mcp, pool);
    Ads1219 ads{ bus, FK_ADS1219_ADDRESS, checker };

    if (!initialize(mcp, ads)) {
        return nullptr;
    }

    auto exciting = excite_enabled();
    auto averaging = averaging_enabled();
    auto priority = fk_task_self_priority_get();
    auto prereading = 0.0f;

    if (exciting) {
        FK_ASSERT(!averaging);
        loginfo("excitation: enabled");

        fk_task_self_priority_set(priority - FK_PRIORITY_HIGH_OFFSET);

        int32_t value = 0;
        if (!ads.read(value)) {
            logerror("read");
            return nullptr;
        }

        prereading = ((float)value * 2.048f) / 8388608.0f;
        loginfo("[%d] water(sample #%d): %f", mc.position().integer(), -1, prereading);

        if (!excite_control(mcp, true)) {
            return nullptr;
        }
    } else {
        loginfo("excitation: disabled");
    }

    constexpr size_t SamplesToAverage = 10;
    constexpr uint32_t AveragingDelayMs = 10;
    size_t samples_to_take = averaging ? SamplesToAverage : 1;
    size_t number_of_values = 0u;
    float accumulator = 0.0f;
    for (auto i = 0u; i < samples_to_take; ++i) {
        int32_t value = 0;
        if (!ads.read(value)) {
            logerror("read");
            return nullptr;
        }

        auto uncalibrated = ((float)value * 2.048f) / 8388608.0f;

        accumulator += uncalibrated;
        number_of_values++;

        if (averaging) {
            loginfo("[%d] water(sample #%d): %f", mc.position().integer(), i, uncalibrated);
            fk_delay(AveragingDelayMs);
        }
    }

    fk_task_self_priority_set(priority);

    auto uncalibrated = accumulator / (float)number_of_values;
    if (exciting) {
        uncalibrated = uncalibrated - pow(prereading, 1.8f);
    }
    auto default_curve = create_modules_default_curve(pool);
    auto curve = create_curve(default_curve, cfg_, pool);
    auto factory = default_curve->apply(uncalibrated);
    auto calibrated = curve->apply(uncalibrated);

    loginfo("[%d] water: %f (%f) (%f)", mc.position().integer(), uncalibrated, calibrated, factory);

    auto has_mpl = header_.kind == FK_MODULES_KIND_WATER_DO;
    ModuleReadings *mr = has_mpl ? (ModuleReadings *)new (pool) NModuleReadings<3>() : (ModuleReadings *)new (pool) NModuleReadings<1>();
    mr->set(0, SensorReading{ mc.now(), uncalibrated, calibrated, factory });

    if (has_mpl) {
        Mpl3115a2 mpl3115a2{ bus };

        if (mpl3115a2.begin()) {
            Mpl3115a2Reading reading;
            if (mpl3115a2.get(&reading)) {
                mr->set(1, SensorReading{ mc.now(), reading.temperature });
                mr->set(2, SensorReading{ mc.now(), reading.pressure });
            } else {
                logerror("mpl3115a2 get");
            }
        } else {
            logerror("mpl3115a2 begin");
        }
    }

#if !defined(FK_WATER_LOCKOUT_ALL_MODULES)
    if (exciting) {
        unlocked_ = uptime + OneMinuteMs;
    }
#else
    unlocked_ = uptime + OneMinuteMs;
#endif

    return mr;
}

bool WaterModule::excite_control(Mcp2803 &mcp, bool high) {
    if (!mcp.configure(FK_MCP2803_IODIR, FK_MCP2803_GPPU, high ? FK_MCP2803_GPIO_EXCITE_ON : FK_MCP2803_GPIO_EXCITE_OFF)) {
        logerror("mcp2803::configure-excite");
        return false;
    }
    return true;
}

} // namespace fk
