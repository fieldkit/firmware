#pragma once

#include <os.h>
#include <utility>

#include "config.h"
#include "common.h"
#include "pool_pointer.h"

namespace fk {

struct TaskDisplayInfo {
    const char *name;
    float progress;
    bool visible;
};

class TaskWorker {
public:
    virtual ~TaskWorker() {
    }

public:
    virtual void run() = 0;

    virtual TaskDisplayInfo display_info() const {
        return {
            .name = name(),
            .progress = 0.0f,
            .visible = true,
        };
    }

    virtual uint8_t priority() const {
        return FK_PRIORITY_NORMAL;
    }

    virtual const char *name() const {
        return "worker";
    }
};

class Worker {
public:
    virtual ~Worker() {
    }

public:
    virtual void run(Pool &pool) = 0;

    virtual uint8_t priority() const {
        return FK_PRIORITY_NORMAL;
    }

    virtual const char *name() const {
        return "worker";
    }

    virtual TaskDisplayInfo display_info() const {
        return {
            .name = name(),
            .progress = 0.0f,
            .visible = true,
        };
    }
};

template <typename Wrapped, typename ConcreteWrapped = Wrapped, class... Args>
class PoolWorker : public TaskWorker, public PoolPointer<Wrapped> {
private:
    Pool *pool_;
    ConcreteWrapped wrapped_;

public:
    PoolWorker(Pool *pool, Args &&...args) : pool_(pool), wrapped_(std::forward<Args>(args)...) {
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
    void run() override {
        wrapped_.run(*pool_);
        alogf(LogLevels::INFO, name(), "pool used = %zd/%zd", pool_->used(), pool_->size());
    }

    uint8_t priority() const override {
        return wrapped_.priority();
    }

    const char *name() const override {
        return wrapped_.name();
    }

    TaskDisplayInfo display_info() const override {
        return wrapped_.display_info();
    }
};

template <typename Wrapped, class... Args> inline TaskWorker *create_pool_worker(Args &&...args) {
    return create_chained_pool_wrapper<Wrapped, TaskWorker, PoolWorker<Wrapped, Wrapped, Args...>, PoolWorker<Wrapped, Wrapped, Args...>>(
        std::forward<Args>(args)...);
}

FK_ENABLE_TYPE_NAME(Worker);

} // namespace fk
