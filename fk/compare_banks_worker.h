#pragma once

#include "worker.h"

namespace fk {

class CompareBanksWorker : public Worker {
public:
    explicit CompareBanksWorker();

public:
    void run(Pool &pool) override;

public:
    uint8_t priority() const override {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() const override {
        return "cmpbanks";
    }
};

FK_ENABLE_TYPE_NAME(CompareBanksWorker);

} // namespace fk
