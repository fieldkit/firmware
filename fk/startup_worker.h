#pragma once

#include "worker.h"

namespace fk {

class StartupWorker : public Worker {
public:
    StartupWorker();
    StartupWorker(Pool *pool);

public:
    void run(Pool &pool) override;

    const char *name() override {
        return "startup";
    }

};

}
