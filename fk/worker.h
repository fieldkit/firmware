#pragma once

#include "common.h"
#include "pool.h"

namespace fk {

struct WorkerContext {
};

class Worker {
private:
    EmptyPool empty_;
    Pool *pool_;

public:
    Worker(Pool *pool) : pool_(pool) {
    }

    virtual ~Worker() {
    }

public:
    virtual void run(WorkerContext &wc, Pool &pool) = 0;


public:
    Pool &pool() {
        if (pool_ == nullptr) {
            return empty_;
        }
        return *pool_;
    }

};

}
