#pragma once

#include "worker.h"

namespace fk {

class StartupWorker {
public:
    void run(Pool &pool);

    const char *name() {
        return "startup";
    }

};

}
