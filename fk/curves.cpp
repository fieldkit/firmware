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

class ExponentialCurve : public Curve {
private:
    float a_{ 0.0f };
    float b_{ 0.0f };

public:
    ExponentialCurve(float a, float b) : a_(a), b_(b) {
    }

public:
    virtual float apply(float uncalibrated) override {
        loginfo("cal(exp): a = %f b = %f", a_, b_);
        return a_ * pow(uncalibrated, b_);
    }
};

Curve *create_noop_curve(Pool &pool) {
    return new (pool) NoopCurve();
}

Curve *create_curve(fk_data_CurveType curve_type, float c0, float c1, Pool &pool) {
    switch (curve_type) {
    case fk_data_CurveType_CURVE_LINEAR: {
        return new (pool) LinearCurve(c0, c1);
    }
    case fk_data_CurveType_CURVE_EXPONENTIAL: {
        return new (pool) ExponentialCurve(c0, c1);
    }
    default: {
        logerror("unexpected curve-type (%d)", curve_type);
        return new (pool) NoopCurve();
    }
    }
}

Curve *create_curve(Curve *default_curve, fk_data_ModuleConfiguration *cfg, Pool &pool) {
    if (cfg == nullptr) {
        loginfo("using default curve: no configuration");
        return default_curve;
    }

    if (!cfg->has_calibration) {
        loginfo("using default curve: no calibration");
        return default_curve;
    }

    if (!cfg->calibration.has_coefficients) {
        loginfo("using default curve: no coefficients");
        return default_curve;
    }

    if (cfg->calibration.coefficients.values.arg == nullptr) {
        loginfo("using default curve: malformed coefficients (none)");
        return default_curve;
    }

    auto curve_type = cfg->calibration.type;
    auto values_array = reinterpret_cast<pb_array_t *>(cfg->calibration.coefficients.values.arg);
    if (values_array->length != 2) {
        loginfo("using default curve: malformed coefficients (number)");
        return default_curve;
    }

    if (cfg->calibration.points.arg != nullptr) {
        auto points_array = reinterpret_cast<pb_array_t *>(cfg->calibration.points.arg);
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

    auto values = reinterpret_cast<float *>(values_array->buffer);
    return create_curve(curve_type, values[0], values[1], pool);
}

} // namespace fk
