#pragma once

#include "common.h"
#include "pool.h"

namespace fk {

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
    virtual void run(Pool &pool) = 0;

    virtual const char *name() {
        return "worker";
    }

public:
    Pool &pool() {
        if (pool_ == nullptr) {
            return empty_;
        }
        return *pool_;
    }

};

}
