#pragma once

#include "worker.h"

namespace fk {

class ReadingsWorker {
public:
    void run(Pool &pool);

    const char *name() {
        return "livedata";
    }

};

}
