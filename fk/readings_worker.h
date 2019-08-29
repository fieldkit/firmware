#pragma once

#include "worker.h"

namespace fk {

class ReadingsWorker : public Worker {
public:
    ReadingsWorker();
    ReadingsWorker(Pool *pool);

public:
    void run(Pool &pool) override;

};

}
