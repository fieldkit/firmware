#pragma once

#include "pool.h"
#include "records.h"

namespace fk {

class Curve {
public:
    virtual float apply(float uncalibrated) = 0;
};

Curve *create_noop_curve(Pool &pool);

Curve *create_curve(fk_data_CurveType curve_type, float const *coefficients, Pool &pool);

Curve *create_curve(Curve *default_curve, fk_data_ModuleConfiguration *cfg, Pool &pool);

} // namespace fk
