#pragma once

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
    PoolWorker(T *wrapped, uint8_t *p, size_t size) : wrapped_(wrapped), pool_(__PRETTY_FUNCTION__, p, size) {
    }

public:
    void operator delete(void *p) {
    }

public:
    void run() override {
        wrapped_->run(pool_);
    }

    const char *name() override {
        return wrapped_->name();
    }
};

template<typename T>
inline PoolWorker<T> *create_pool_worker(size_t size) {
    auto block = (uint8_t *)malloc(size);

    auto worker_size = aligned_size(sizeof(T));
    auto worker = (T *)block;
    new (worker) T();

    auto pool_worker_size = aligned_size(sizeof(PoolWorker<T>));
    auto pool_worker = (PoolWorker<T> *)(block + worker_size);
    auto overhead = pool_worker_size + pool_worker_size;

    auto pool_memory = (block + overhead);
    new (pool_worker) PoolWorker<T>(worker, pool_memory, size - overhead);

    return pool_worker;
}

template<typename T, class... Args>
inline PoolWorker<T> *create_pool_worker(size_t size, Args &&... args) {
    auto block = (uint8_t *)malloc(size);

    auto worker_size = aligned_size(sizeof(T));
    auto worker = (T *)block;
    new (worker) T(std::forward<Args>(args)...);

    auto pool_worker_size = aligned_size(sizeof(PoolWorker<T>));
    auto pool_worker = (PoolWorker<T> *)(block + worker_size);
    auto overhead = pool_worker_size + pool_worker_size;

    auto pool_memory = (block + overhead);
    new (pool_worker) PoolWorker<T>(worker, pool_memory, size - overhead);

    return pool_worker;
}

}
