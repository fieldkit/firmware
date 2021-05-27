#pragma once

#include "worker.h"

namespace fk {

class FactoryWipeWorker : public Worker {
private:
    bool restart_{ true };

public:
    FactoryWipeWorker(bool restart);

public:
    void run(Pool &pool) override;

public:
    const char *name() const override {
        return "wipe";
    }

private:
    bool initialize_memory_if_necessary();

};

FK_ENABLE_TYPE_NAME(FactoryWipeWorker);

}
