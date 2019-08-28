#pragma once

#include "common.h"
#include "pool.h"
#include "state_ref.h"

namespace fk {

class GlobalStateManager {
public:
    bool initialize(Pool &pool);
    bool rebuild();

    template<typename T>
    bool apply(T fn) {
        auto gs = get_global_state_rw();
        fn(gs.get());
        return true;
    }

};

}
