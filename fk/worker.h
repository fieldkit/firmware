#pragma once

#include <os.h>
#include <utility>

#include "config.h"
#include "common.h"
#include "pool_pointer.h"

namespace fk {

class Worker {
public:
    virtual ~Worker() {
    }

public:
    virtual void run(Pool &pool) = 0;

    virtual uint8_t priority() const {
        return OS_PRIORITY_NORMAL;
    }

    virtual const char *name() {
        return "worker";
    }

};

template<typename Wrapped, typename ConcreteWrapped = Wrapped, class... Args>
class PoolWorker : public Worker, public PoolPointer<Wrapped>  {
private:
    Pool *pool_;
    ConcreteWrapped wrapped_;

public:
    PoolWorker(Pool *pool, Args&&... args) : pool_(pool), wrapped_(std::forward<Args>(args)...) {
    }

    virtual ~PoolWorker() {
        delete pool_;
    }

public:
    void operator delete(void *p) {
        // We are freed automatically when the pool is deleted in our
        // destructor. So we don't need to do anything in here.
    }

public:
    Pool *pool() override {
        return pool_;
    }

    Wrapped *get() override {
        return &wrapped_;
    }


public:
    void run(Pool &pool) override {
        wrapped_.run(*pool_);
        alogf(LogLevels::INFO, name(), "pool used = %zd/%zd", pool_->used(), pool_->size());
    }

    uint8_t priority() const override {
        return wrapped_.priority();
    }

    const char *name() override {
        return wrapped_.name();
    }
};

template<typename Wrapped, typename Wrapee = PoolWorker<Wrapped>, size_t Size = DefaultWorkerPoolSize, class... Args>
inline Worker *create_pool_worker(Args &&... args) {
    return create_chained_pool_wrapper<Wrapped, Worker, Wrapee, Wrapee>(std::forward<Args>(args)...);
}

}
