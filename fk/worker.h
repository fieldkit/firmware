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
    Worker() : pool_(nullptr) {
    }

    Worker(Pool *pool) : pool_(pool) {
    }

    virtual ~Worker() {
        if (pool_ != nullptr) {
            delete pool_;
        }
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
