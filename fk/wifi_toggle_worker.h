#pragma once

#include "worker.h"

namespace fk {

class WifiToggleWorker {
public:
    enum class DesiredState {
        Enabled,
        Disabled,
        Toggle,
        Restart,
    };

private:
    DesiredState desired_;

public:
    WifiToggleWorker(DesiredState desired = DesiredState::Toggle);

public:
    void run(Pool &pool);

    uint8_t priority() const {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() {
        return "wifi";
    }

};

}
