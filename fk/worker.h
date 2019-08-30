#pragma once

#include <os.h>
#include <utility>

#include "common.h"
#include "pool.h"

namespace fk {

class Worker {
public:
    virtual ~Worker() {
    }

public:
    virtual void run() = 0;

    virtual uint8_t priority() const {
        return OS_PRIORITY_NORMAL;
    }

    virtual const char *name() {
        return "worker";
    }

};

template<typename T>
class PoolWorker : public Worker {
private:
    MallocPool pool_;
    T *wrapped_;

public:
    PoolWorker(uint8_t *p, size_t size, size_t taken) :
        pool_(__PRETTY_FUNCTION__, p, size, taken) {
    }

    template<class... Args>
    T *create(Args &&... args) {
        wrapped_ = new (pool_) T(std::forward<Args>(args)...);
        return wrapped_;
    }

    virtual ~PoolWorker() {
        pool_.block(nullptr, 0);
    }

public:
    void operator delete(void *p) {
        fk_free(p);
    }

public:
    void run() override {
        wrapped_->run(pool_);
    }

    uint8_t priority() const override {
        return wrapped_->priority();
    }

    const char *name() override {
        return wrapped_->name();
    }
};

}
