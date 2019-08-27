#pragma once

#include "common.h"
#include "pool.h"

namespace fk {

class GlobalStateManager {
public:
    bool initialize(Pool &pool);
    bool rebuild();

};

}
