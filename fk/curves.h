#pragma once

#include "pool.h"
#include "records.h"

namespace fk {

class Curve {
public:
    virtual float apply(float uncalibrated) = 0;
};

Curve *create_curve(fk_data_ModuleConfiguration *cfg, Pool &pool);

} // namespace fk
