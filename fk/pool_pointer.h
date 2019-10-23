#pragma once

#include <utility>

#include "pool.h"

namespace fk {

template<typename T>
class PoolPointer {
public:
    virtual ~PoolPointer() {
    }

public:
    virtual Pool *pool() = 0;
    virtual T *get() = 0;

};

template<typename T>
class PoolWrapper : public PoolPointer<T> {
private:
    MallocPool pool_;
    T *wrapped_;

public:
    PoolWrapper(uint8_t *p, size_t size, size_t taken) :
        pool_(__PRETTY_FUNCTION__, p, size, taken) {
    }

    virtual ~PoolWrapper() {
        pool_.block(nullptr, 0);
    }

public:
    void operator delete(void *p) {
        fk_free(p);
    }

public:
    void wrapped(T *wrapped) {
        wrapped_ = wrapped;
    }

public:
    Pool *pool() override {
        return &pool_;
    }

    T *get() override {
        return wrapped_;
    }

};

template<typename T, typename C = T, typename W = PoolWrapper<T>, size_t Size = DefaultWorkerPoolSize, class... Args>
inline W *create_pool_wrapper(Args &&... args) {
    auto block = (uint8_t *)fk_malloc(Size);
    FK_ASSERT(block != nullptr);

    auto wrapper_size = aligned_size(sizeof(W));
    auto overhead = wrapper_size;
    auto pool_memory = block + overhead;

    auto wrapper = new ((W *)(block)) W(pool_memory, Size, overhead);

    auto wrapped = new (wrapper->pool()) C(std::forward<Args>(args)...);

    wrapper->wrapped(wrapped);

    return wrapper;
}

}
