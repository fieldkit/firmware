#include "water_protocol.h"
#include "state_ref.h"
#include "ready_checkers.h"

namespace fk {

FK_DECLARE_LOGGER("waterproto");

#define FK_MCP2803_IODIR 0b00000010
#define FK_MCP2803_GPPU  0b00000010

#define FK_MCP2803_GPIO_ON  0b00000001
#define FK_MCP2803_GPIO_OFF 0b00000000

#define FK_MCP2803_GPIO_EXCITE_ON  0b00000101
#define FK_MCP2803_GPIO_EXCITE_OFF 0b00000001

WaterProtocol::WaterProtocol(Pool &pool, TwoWireWrapper &bus, WaterModality modality) : pool_(pool), bus_(bus), modality_(modality) {
    if (excite_enabled()) {
        readings_checker_ =
            new (pool) UnexciteBeforeReadyChecker{ mcp_, Mcp2803Config{ FK_MCP2803_IODIR, FK_MCP2803_GPPU, FK_MCP2803_GPIO_EXCITE_OFF } };
    }

    readings_checker_ = new (pool) Mcp2803ReadyChecker{ mcp_ };
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
    if (!mcp_.configure(FK_MCP2803_IODIR, FK_MCP2803_GPPU, FK_MCP2803_GPIO_OFF)) {
        logerror("mcp2803::begin");
        return false;
    }

    if (!mcp_.configure(FK_MCP2803_IODIR, FK_MCP2803_GPPU, FK_MCP2803_GPIO_ON)) {
        logerror("mcp2803::begin");
        return false;
    }

    fk_delay(100);

    if (!ads_.begin()) {
        logerror("ads1219::begin");
        return false;
    }

    if (!ads_.configure(Ads1219VoltageReference::Internal, Ads1219Channel::Diff_0_1, Ads1219Gain::One, Ads1219DataRate::DataRate_1000)) {
        logerror("ads1219::configure");
        return false;
    }

    return true;
}

bool WaterProtocol::excite_control(bool high) {
    if (!mcp_.configure(FK_MCP2803_IODIR, FK_MCP2803_GPPU, high ? FK_MCP2803_GPIO_EXCITE_ON : FK_MCP2803_GPIO_EXCITE_OFF)) {
        logerror("mcp2803::configure-excite");
        return false;
    }

    return true;
}

WaterReadings *WaterProtocol::take_readings(ReadingsContext mc, fk_data_ModuleConfiguration *cfg, Pool &pool) {
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
    auto curve = create_curve(default_curve, cfg, pool);
    auto factory = default_curve->apply(uncalibrated);
    auto calibrated = curve->apply(uncalibrated);

    loginfo("[%d] water: %f (%f) (%f)", mc.position().integer(), uncalibrated, calibrated, factory);

    return new (pool) WaterReadings{ uncalibrated, calibrated, factory };
}

bool WaterProtocol::block_until_ready(TwoWireWrapper &bus) {
    return readings_checker_->block_until_ready(bus);
}

} // namespace fk