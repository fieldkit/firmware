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

Curve *create_curve(Curve *default_curve, fk_data_Calibration *calibration, Pool &pool) {
    if (!calibration->has_coefficients) {
        logwarn("using default curve: no coefficients");
        return default_curve;
    }

    if (calibration->coefficients.values.arg == nullptr) {
        logwarn("using default curve: malformed coefficients (none)");
        return default_curve;
    }

    if (calibration->points.arg != nullptr) {
        auto points_array = reinterpret_cast<pb_array_t *>(calibration->points.arg);
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
        logwarn("curve missing points");
    }

    auto expected_coefficients = 0u;

    switch (calibration->type) {
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
        logwarn("using default curve: unexpected curve-type (%d)", calibration->type);
        return default_curve;
    }
    }

    auto curve_type = calibration->type;
    auto values_array = reinterpret_cast<pb_array_t *>(calibration->coefficients.values.arg);
    if (values_array->length != expected_coefficients) {
        logwarn("using default curve: malformed coefficients (%d != %d)", values_array->length != expected_coefficients);
        return default_curve;
    }

    float coefficients[3] = { 0, 0, 0 };
    auto values = reinterpret_cast<float *>(values_array->buffer);
    for (auto i = 0u; i < values_array->length; ++i) {
        coefficients[i] = values[i];
    }

    return create_curve(curve_type, coefficients, pool);
}

Curve *create_curve(Curve *default_curve, uint32_t kind, fk_data_ModuleConfiguration *cfg, Pool &pool) {
    if (cfg == nullptr) {
        loginfo("using default curve: no configuration");
        return default_curve;
    }

    if (cfg->calibrations.arg != nullptr) {
        auto calibrations_array = reinterpret_cast<pb_array_t *>(cfg->calibrations.arg);
        auto calibrations = reinterpret_cast<fk_data_Calibration *>(calibrations_array->buffer);
        for (auto i = 0u; i < calibrations_array->length; ++i) {
            if (calibrations[i].kind == kind) {
                loginfo("curve: found kind among calibrations");
                return create_curve(default_curve, &calibrations[i], pool);
            } else {
                logdebug("using default curve: skipping (%d != %d)", calibrations[i].kind, kind);
            }
        }
        loginfo("using default curve: no calibration of kind (%d) (length=%zu)", kind, calibrations_array->length);
    } else {
        loginfo("using default curve: no calibrations");
    }

    if (!cfg->has_calibration) {
        loginfo("using default curve: no calibration");
        return default_curve;
    }

    return create_curve(default_curve, &cfg->calibration, pool);
}

} // namespace fk
