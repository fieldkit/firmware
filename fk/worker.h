#pragma once

#include "common.h"

namespace fk {

class WorkerContext {
};

class Worker {
public:
    virtual void run(WorkerContext &wc) = 0;

};

}
