#pragma once

#include "worker.h"
#include "readings.h"
#include "storage/storage.h"

namespace fk {

class ServiceModulesWorker : public Worker {
public:
    ServiceModulesWorker();

public:
    void run(Pool &pool) override;

    uint8_t priority() const override {
        return OS_PRIORITY_NORMAL;
    }

    const char *name() const override {
        return "modsvc";
    }

};

FK_ENABLE_TYPE_NAME(ServiceModulesWorker);

}
