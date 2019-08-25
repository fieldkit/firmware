#pragma once

#include "worker.h"

namespace fk {

class ReadingsWorker : public Worker {
public:
    void run(WorkerContext &wc) override;

};

}
