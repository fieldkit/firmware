#pragma once

#include "worker.h"
#include "self_check.h"

namespace fk {

class SelfCheckWorker : public Worker {
private:
    SelfCheckCallbacks *callbacks_;

public:
    explicit SelfCheckWorker(SelfCheckCallbacks &callbacks);

public:
    void run(Pool &pool) override;

    const char *name() const override {
        return "self-check";
    }

private:
    bool stop_network();
    void check();

};

FK_ENABLE_TYPE_NAME(SelfCheckWorker);

}
