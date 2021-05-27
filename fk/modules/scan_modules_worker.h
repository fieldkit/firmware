#pragma once

#include "worker.h"

namespace fk {

class ScanModulesWorker : public Worker {
public:
    ScanModulesWorker();

public:
    void run(Pool &pool) override;

public:
    const char *name() const override {
        return "modscan";
    }

};

FK_ENABLE_TYPE_NAME(ScanModulesWorker);

}
