#pragma once

#include "common.h"
#include "pool.h"
#include "state_ref.h"
#include "platform.h"

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

    bool notify(NotificationState notification) {
        return apply([=](GlobalState *gs) {
            gs->notification = notification;
            gs->notification.created = fk_uptime();
        });
    }

};

}
