#pragma once

#include "worker.h"

namespace fk {

class ReadingsWorker : public Worker {
public:
    ReadingsWorker(Pool *pool) : Worker(pool) {
    }

public:
    void run(WorkerContext &wc, Pool &pool) override;

};

}
