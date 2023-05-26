#pragma once

#include "pool.h"
#include "records.h"

namespace fk {

const size_t CalibrationMaximumCalibrations = 5;
const size_t CalibrationMaximumCoefficients = 3;
const size_t CalibrationMaximumPoints = 3;

struct calibration_point_t {
    float uncalibrated;
    float standard;
    float factory;
};

struct calibration_t {
    uint32_t kind;
    fk_data_CurveType type;
    calibration_point_t points[CalibrationMaximumPoints];
    float coefficients[CalibrationMaximumCoefficients];
};

struct calibration_config_t {
    calibration_t calibrations[CalibrationMaximumCalibrations];
};

const size_t WaterConfigSerializedMaximum = 512;

class WaterConfig {
public:
    calibration_config_t cal_;
    EncodedMessage encoded_;
    uint8_t serialized_[WaterConfigSerializedMaximum];

public:
    bool load(std::pair<EncodedMessage *, fk_data_ModuleConfiguration *> loaded);
    EncodedMessage *encoded() {
        return &encoded_;
    }
};

class Curve {
public:
    virtual float apply(float uncalibrated) = 0;
};

Curve *create_noop_curve(Pool &pool);

Curve *create_curve(fk_data_CurveType curve_type, float const *coefficients, Pool &pool);

Curve *create_curve(Curve *default_curve, uint32_t kind, calibration_config_t *cal, Pool &pool);

} // namespace fk
