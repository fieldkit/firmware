#include "curves.h"
#include "common.h"
#include <Arduino.h>
#include <math.h>

namespace fk {

FK_DECLARE_LOGGER("curves");

class NoopCurve : public Curve {
public:
    virtual float apply(float uncalibrated) override {
        loginfo("cal(noop)");
        return uncalibrated;
    }
};

class LinearCurve : public Curve {
private:
    float b_{ 0.0f };
    float m_{ 0.0f };

public:
    LinearCurve(float b, float m) : b_(b), m_(m) {
    }

public:
    virtual float apply(float uncalibrated) override {
        loginfo("cal(linear): b = %f m = %f", b_, m_);
        return b_ + (uncalibrated * m_);
    }
};

class PowerCurve : public Curve {
private:
    float a_{ 0.0f };
    float b_{ 0.0f };

public:
    PowerCurve(float a, float b) : a_(a), b_(b) {
    }

public:
    virtual float apply(float uncalibrated) override {
        loginfo("cal(power): a = %f b = %f", a_, b_);
        return a_ * pow(uncalibrated, b_);
    }
};

class ExponentialCurve : public Curve {
private:
    float a_{ 0.0f };
    float b_{ 0.0f };
    float c_{ 0.0f };

public:
    ExponentialCurve(float a, float b, float c) : a_(a), b_(b), c_(c) {
    }

public:
    virtual float apply(float uncalibrated) override {
        loginfo("cal(exponential): a = %f b = %f c = %f", a_, b_, c_);
        return a_ + b_ * exp(uncalibrated * c_);
    }
};

Curve *create_noop_curve(Pool &pool) {
    return new (pool) NoopCurve();
}

Curve *create_curve(fk_data_CurveType curve_type, float const *coefficients, Pool &pool) {
    switch (curve_type) {
    case fk_data_CurveType_CURVE_LINEAR: {
        return new (pool) LinearCurve(coefficients[0], coefficients[1]);
    }
    case fk_data_CurveType_CURVE_POWER: {
        return new (pool) PowerCurve(coefficients[0], coefficients[1]);
    }
    case fk_data_CurveType_CURVE_EXPONENTIAL: {
        return new (pool) ExponentialCurve(coefficients[0], coefficients[1], coefficients[2]);
    }
    default: {
        logerror("unexpected curve-type (%d)", curve_type);
        return new (pool) NoopCurve();
    }
    }
}

Curve *create_curve(Curve *default_curve, uint32_t kind, calibration_config_t *cal, Pool &pool) {
    if (cal == nullptr) {
        loginfo("using default curve: no configuration");
        return default_curve;
    }

    for (auto i = 0u; i < CalibrationMaximumCalibrations; ++i) {
        auto &calibration = cal->calibrations[i];
        if (calibration.kind == kind) {
            loginfo("curve: found!");
            return create_curve(calibration.type, calibration.coefficients, pool);
        } else {
            logdebug("curve: skipping (%d != %d)", kind, cal->calibrations[i].kind);
        }
    }

    loginfo("using default curve: no calibration");
    return default_curve;
}

bool fill_calibration(fk_data_Calibration *cfg, calibration_t *cal) {
    if (!cfg->has_coefficients) {
        logwarn("load-cal: no coefficients");
        return false;
    }

    if (cfg->coefficients.values.arg == nullptr) {
        logwarn("load-cal: malformed coefficients (none)");
        return false;
    }

    if (cfg->points.arg != nullptr) {
        auto points_array = reinterpret_cast<pb_array_t *>(cfg->points.arg);
        auto points = reinterpret_cast<fk_data_CalibrationPoint *>(points_array->buffer);
        for (auto i = 0u; i < points_array->length; ++i) {
            auto uncalibrated_array = reinterpret_cast<pb_array_t *>(points[i].uncalibrated.arg);
            auto references_array = reinterpret_cast<pb_array_t *>(points[i].references.arg);
            auto factory_array = reinterpret_cast<pb_array_t *>(points[i].factory.arg);
            auto uncalibrated = reinterpret_cast<float *>(uncalibrated_array->buffer);
            auto references = reinterpret_cast<float *>(references_array->buffer);
            auto factory = reinterpret_cast<float *>(factory_array->buffer);
            loginfo("curve[%d]: uncal=%f ref=%f factory=%f", i, uncalibrated[0], references[0], factory[0]);
        }
    } else {
        logwarn("load-cal: curve missing points");
    }

    auto expected_coefficients = 0u;

    switch (cfg->type) {
    case fk_data_CurveType_CURVE_LINEAR: {
        expected_coefficients = 2u;
        break;
    }
    case fk_data_CurveType_CURVE_POWER: {
        expected_coefficients = 2u;
        break;
    }
    case fk_data_CurveType_CURVE_EXPONENTIAL: {
        expected_coefficients = 3u;
        break;
    }
    default: {
        logwarn("load-cal: unexpected curve-type (%d)", cfg->type);
        return false;
    }
    }

    auto curve_type = cfg->type;
    auto values_array = reinterpret_cast<pb_array_t *>(cfg->coefficients.values.arg);
    if (values_array->length != expected_coefficients) {
        logwarn("load-cal: malformed coefficients (%d != %d)", values_array->length != expected_coefficients);
        return false;
    }

    auto values = reinterpret_cast<float *>(values_array->buffer);
    for (auto i = 0u; i < values_array->length; ++i) {
        cal->coefficients[i] = values[i];
    }
    cal->type = curve_type;
    cal->kind = cfg->kind;

    return true;
}

bool fill_calibration_config(fk_data_ModuleConfiguration *cfg, calibration_config_t *cal) {
    memzero(cal, sizeof(calibration_config_t));

    if (cfg == nullptr) {
        loginfo("load-cal: no configuration");
        return false;
    }

    if (cfg->has_calibration) {
        loginfo("load-cal: single");
        return fill_calibration(&cfg->calibration, &cal->calibrations[0]);
    }

    if (cfg->calibrations.arg != nullptr) {
        loginfo("load-cal: multiple");
        auto calibrations_array = reinterpret_cast<pb_array_t *>(cfg->calibrations.arg);
        auto calibrations = reinterpret_cast<fk_data_Calibration *>(calibrations_array->buffer);
        for (auto i = 0u; i < calibrations_array->length; ++i) {
            if (!fill_calibration(&calibrations[i], &cal->calibrations[i])) {
                return false;
            }
        }
        return true;
    }

    return false;
}

bool WaterConfig::load(std::pair<EncodedMessage *, fk_data_ModuleConfiguration *> loaded) {
    memzero(&cal_, sizeof(cal_));
    memzero(serialized_, WaterConfigSerializedMaximum);
    encoded_ = EncodedMessage{};

    if (loaded.first != nullptr) {
        if (!fill_calibration_config(loaded.second, &cal_)) {
            logwarn("configuration error");
        }

        auto size = loaded.first->size;
        auto buffer = loaded.first->buffer;
        FK_ASSERT(size <= WaterConfigSerializedMaximum);
        memcpy(serialized_, buffer, size);
        encoded_ = EncodedMessage{ size, buffer };
    }

    return true;
}

} // namespace fk
