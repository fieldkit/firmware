#pragma once

#include "worker.h"
#include "storage/storage.h"

namespace fk {

class ServiceModulesWorker : public Worker {
public:
    ServiceModulesWorker();

public:
    void run(Pool &pool) override;

    const char *name() const override {
        return "modsvc";
    }

};

FK_ENABLE_TYPE_NAME(ServiceModulesWorker);

}
