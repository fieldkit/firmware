#pragma once

#include "worker.h"

namespace fk {

enum class ConfigureModuleKind {
    Weather,
    Water,
    Ultrasonic,
};

class ConfigureModuleWorker {
private:
    ConfigureModuleKind kind_;

public:
    ConfigureModuleWorker(ConfigureModuleKind kind);

public:
    void run(Pool &pool);

    uint8_t priority() const {
        return OS_PRIORITY_NORMAL;
    }

    const char *name() {
        return "cfgworker";
    }

};

}
