#pragma once

#include "worker.h"
#include "readings.h"
#include "storage/storage.h"

namespace fk {

class ServiceModulesWorker {
public:
    ServiceModulesWorker();

public:
    void run(Pool &pool);

    uint8_t priority() const {
        return OS_PRIORITY_NORMAL;
    }

    const char *name() {
        return "modsvc";
    }

};

FK_ENABLE_TYPE_NAME(ServiceModulesWorker);

}
