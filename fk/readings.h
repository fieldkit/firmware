#pragma once

#include <fk-data-protocol.h>

#include "registry.h"
#include "state.h"

namespace fk {

class Readings {
private:
    GlobalState const *gs_;
    fk_data_DataRecord record_;

public:
    Readings(GlobalState const *gs);

public:
    bool take_readings(ResolvedModules const &modules, uint32_t reading_number, Pool &pool);

public:
    fk_data_DataRecord &record();

};


}
