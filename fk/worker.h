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
    T *wrapped_;
    MallocPool pool_;

public:
    PoolWorker(T *wrapped, uint8_t *p, size_t size, size_t taken) : wrapped_(wrapped), pool_(__PRETTY_FUNCTION__, p, size, taken) {
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

template<typename T>
inline PoolWorker<T> *create_pool_worker(size_t size) {
    auto block = (uint8_t *)fk_malloc(size);

    auto worker_size = aligned_size(sizeof(T));
    auto pool_worker_size = aligned_size(sizeof(PoolWorker<T>));
    auto overhead = pool_worker_size + worker_size;
    auto pool_memory = block + overhead;

    auto pool_worker = (PoolWorker<T> *)(block);
    auto worker = (T *)(block + pool_worker_size);

    new (worker) T();

    new (pool_worker) PoolWorker<T>(worker, pool_memory, size, 0);

    return pool_worker;
}

template<typename T, class... Args>
inline PoolWorker<T> *create_pool_worker(size_t size, Args &&... args) {
    auto block = (uint8_t *)fk_malloc(size);

    auto worker_size = aligned_size(sizeof(T));
    auto pool_worker_size = aligned_size(sizeof(PoolWorker<T>));
    auto overhead = pool_worker_size + worker_size;
    auto pool_memory = block + overhead;

    auto pool_worker = (PoolWorker<T> *)(block);
    auto worker = (T *)(block + pool_worker_size);

    new (worker) T(std::forward<Args>(args)...);

    new (pool_worker) PoolWorker<T>(worker, pool_memory, size, 0);

    return pool_worker;
}

}
