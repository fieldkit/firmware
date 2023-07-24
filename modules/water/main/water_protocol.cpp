#include "water_protocol.h"
#include "state_ref.h"
#include "ready_checkers.h"

namespace fk {

FK_DECLARE_LOGGER("waterproto");

WaterProtocol::WaterProtocol(Pool &pool, TwoWireWrapper &bus, WaterModality modality, WaterMcpGpioConfig mcp_config, bool standalone_orp)
    : pool_(pool), bus_(bus), modality_(modality), mcp_config_(mcp_config), standalone_orp_(standalone_orp) {
    if (excite_enabled()) {
        readings_checker_ =
            new (pool) UnexciteBeforeReadyChecker{ mcp_, Mcp2803Config{ mcp_config_.io_dir, mcp_config_.pullups, mcp_config_.excite_off } };
    } else {
        readings_checker_ = new (pool) Mcp2803ReadyChecker{ mcp_ };
    }
}

bool WaterProtocol::averaging_enabled() const {
    switch (modality_) {
    case WaterModality::DO: {
        return true;
    }
    default:
        return false;
    };
}

bool WaterProtocol::excite_enabled() const {
    switch (modality_) {
    case WaterModality::EC: {
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

bool WaterProtocol::lockout_enabled() const {
    switch (modality_) {
    case WaterModality::EC: {
        return true;
    }
    case WaterModality::PH: {
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
Curve *WaterProtocol::create_modules_default_curve(Pool &pool) {
    switch (modality_) {
    case WaterModality::Temp: {
        constexpr float TempDefaultCalibration[3]{ -900.53, 662.56, 0 };
        return create_curve(fk_data_CurveType_CURVE_LINEAR, TempDefaultCalibration, pool);
    }
    case WaterModality::PH: {
        constexpr float PhDefaultCalibration[3] = { 15.992, -17.777, 0 };
        return create_curve(fk_data_CurveType_CURVE_LINEAR, PhDefaultCalibration, pool);
    }
    case WaterModality::DO: {
        constexpr float DoDefaultCalibration[3] = { 16.663, 2202.1, 0 };
        return create_curve(fk_data_CurveType_CURVE_LINEAR, DoDefaultCalibration, pool);
    }
    case WaterModality::ORP: {
        constexpr float OrpDefaultCalibration[3] = { 0, 1000, 0 };
        return create_curve(fk_data_CurveType_CURVE_LINEAR, OrpDefaultCalibration, pool);
    }
    case WaterModality::EC: {
        // constexpr float EcDefaultCalibration_03252022_0000[3] = { 1013.407233, 235718422.3, -10.66457333 };
        // constexpr float EcDefaultCalibration_03252022_1500[3] = { -227.6927, 116077.5333, -3.049790667 };
        constexpr float EcDefaultCalibration_04292022_1141[3] = { 1032.49022, 5432917.214, -8.227149468 };
        return create_curve(fk_data_CurveType_CURVE_EXPONENTIAL, EcDefaultCalibration_04292022_1141, pool);
    }
    default:
        return create_noop_curve(pool);
    };
}

bool WaterProtocol::initialize() {
    if (!mcp_.configure(mcp_config_.io_dir, mcp_config_.pullups, mcp_config_.off)) {
        logerror("mcp2803::begin");
        return false;
    }

    if (!mcp_.configure(mcp_config_.io_dir, mcp_config_.pullups, mcp_config_.on)) {
        logerror("mcp2803::begin");
        return false;
    }

    fk_delay(100);

    if (!ads_.begin()) {
        logerror("ads1219::begin");
        return false;
    }

    auto channel = Ads1219Channel::Diff_0_1;

    switch (modality_) {
    case Temp:
        channel = Ads1219Channel::Single_0;
        break;
    case PH:
        channel = Ads1219Channel::Single_0;
        break;
    case EC:
        channel = Ads1219Channel::Single_0;
        break;
    case DO:
        channel = Ads1219Channel::Diff_0_1;
        break;
    case ORP:
        if (standalone_orp_) {
            channel = Ads1219Channel::Diff_0_1; // Standalone
        } else {
            channel = Ads1219Channel::Diff_2_3; // Omni
        }
        break;
    };

    if (!ads_.configure(Ads1219VoltageReference::Internal, channel, Ads1219Gain::One, Ads1219DataRate::DataRate_1000)) {
        logerror("ads1219::configure");
        return false;
    }

    return true;
}

bool WaterProtocol::excite_control(bool high) {
    if (!mcp_.configure(mcp_config_.io_dir, mcp_config_.pullups, high ? mcp_config_.excite_on : mcp_config_.excite_off)) {
        logerror("mcp2803::configure-excite");
        return false;
    }

    return true;
}

static uint32_t get_kind_from_modality(WaterModality m) {
    switch (m) {
    case Temp:
        return FK_MODULES_KIND_WATER_TEMP;
    case PH:
        return FK_MODULES_KIND_WATER_PH;
    case EC:
        return FK_MODULES_KIND_WATER_EC;
    case DO:
        return FK_MODULES_KIND_WATER_DO;
    case ORP:
        return FK_MODULES_KIND_WATER_ORP;
    }
    return 0;
}

WaterReadings *WaterProtocol::take_readings(ReadingsContext mc, calibration_config_t *cal, Pool &pool) {
    auto exciting = excite_enabled();
    auto averaging = averaging_enabled();
    auto priority = fk_task_self_priority_get();
    auto prereading = 0.0f;

    if (exciting) {
        FK_ASSERT(!averaging);
        loginfo("excitation: enabled");

        fk_task_self_priority_set(priority - FK_PRIORITY_HIGH_OFFSET);

        int32_t value = 0;
        if (!ads_.read(value)) {
            logerror("read");
            return nullptr;
        }

        prereading = ((float)value * 2.048f) / 8388608.0f;
        loginfo("[%d] water(sample #%d): %f", mc.position().integer(), -1, prereading);

        if (!excite_control(true)) {
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
        if (!ads_.read(value)) {
            logerror("read");
            return nullptr;
        }

        auto uncalibrated = ((float)value * 2.048f) / 8388608.0f;

        accumulator += uncalibrated;
        number_of_values++;

        loginfo("[%d] water(sample #%d): %f", mc.position().integer(), i, uncalibrated);

        if (averaging) {
            fk_delay(AveragingDelayMs);
        }
    }

    fk_task_self_priority_set(priority);

    auto uncalibrated = accumulator / (float)number_of_values;
    if (exciting) {
        if (prereading > 0) {
            float compensated = uncalibrated - pow(prereading, 1.8f);
            loginfo("[%d] water: r=%f p=%f c=%f", mc.position().integer(), uncalibrated, prereading, compensated);
            uncalibrated = compensated;
        } else {
            loginfo("[%d] water: r=%f p=%f (negative prereading)", mc.position().integer(), uncalibrated, prereading);
        }
    }
    auto default_curve = create_modules_default_curve(pool);
    auto curve = create_curve(default_curve, get_kind_from_modality(modality_), cal, pool);
    auto factory = default_curve->apply(uncalibrated);
    auto calibrated = curve->apply(uncalibrated);

    loginfo("[%d] water: %f (%f) (%f)", mc.position().integer(), uncalibrated, calibrated, factory);

    return new (pool) WaterReadings{ uncalibrated, calibrated, factory };
}

bool WaterProtocol::block_until_ready(TwoWireWrapper &bus) {
    return readings_checker_->block_until_ready(bus);
}

} // namespace fk