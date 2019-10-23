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

    virtual ~PoolWorker() {
        alogf(LogLevels::DEBUG, "debug", "0x%p ~PoolWorker.1", this);
        pool_.block(nullptr, 0);
        alogf(LogLevels::DEBUG, "debug", "0x%p ~PoolWorker.2", this);
    }

public:
    void operator delete(void *p) {
        alogf(LogLevels::DEBUG, "debug", "~PoolWorker.delete(0x%p)", p);
        fk_free(p);
        alogf(LogLevels::DEBUG, "debug", "~PoolWorker.delete(0x%p)", p);
    }

public:
    void wrapped(T *wrapped) {
        wrapped_ = wrapped;
    }

    MallocPool &pool() {
        return pool_;
    }

public:
    void run() override {
        wrapped_->run(pool_);
        alogf(LogLevels::INFO, name(), "pool used = %zd/%zd", pool().used(), pool().size());
    }

    uint8_t priority() const override {
        return wrapped_->priority();
    }

    const char *name() override {
        return wrapped_->name();
    }
};

template<typename T, typename W = PoolWorker<T>, size_t Size = DefaultWorkerPoolSize, class... Args>
inline W *create_pool_worker(Args &&... args) {
    return create_pool_wrapper<T, T, W, Size>(std::forward<Args>(args)...);
}

}
