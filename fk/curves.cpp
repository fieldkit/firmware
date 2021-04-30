#include "curves.h"
#include "common.h"

namespace fk {

FK_DECLARE_LOGGER("curves");

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

Curve *create_curve(fk_data_ModuleConfiguration *cfg, Pool &pool) {
    if (cfg == nullptr)
        return new (pool) NoopCurve();
    if (cfg->calibration.type == fk_data_CurveType_CURVE_LINEAR)
        return new (pool) LinearCurve(cfg);
    return new (pool) NoopCurve();
}

} // namespace fk
