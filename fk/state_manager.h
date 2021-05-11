#pragma once

#include "common.h"
#include "pool.h"
#include "state_ref.h"

namespace fk {

class GlobalStateManager {
public:
    bool initialize(Pool &pool);
    bool rebuild();

    template<typename RT>
    RT read(std::function<RT(GlobalState*)> fn) {
        auto gs = get_global_state_ro();
        return fn(gs.get());
    }

    template<typename T>
    bool apply(T fn) {
        auto gs = get_global_state_rw();
        fn(gs.get());
        return true;
    }

    bool notify(NotificationState notification) {
        return apply([=](GlobalState *gs) {
            gs->notification = notification;
        });
    }

    bool notify(const char *message) {
        return notify(NotificationState{ message });
    }

};

}
