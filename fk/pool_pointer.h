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

    template<typename K>
    K *get() {
        return reinterpret_cast<K*>(get());
    }

};

template<typename Wrapped, typename ConcreteWrapped = Wrapped, class... Args>
class ChainedPoolWrapper : public PoolPointer<Wrapped> {
private:
    Pool *pool_;
    ConcreteWrapped wrapped_;

public:
    ChainedPoolWrapper(Pool *pool, Args&&... args) : pool_(pool), wrapped_(std::forward<Args>(args)...) {
    }

    virtual ~ChainedPoolWrapper() {
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

};

template<typename Wrapped, typename Wrapee = PoolPointer<Wrapped>, typename ConcreteWrapped = Wrapped, typename ConcreteWrapee = ChainedPoolWrapper<Wrapped, ConcreteWrapped>, class... Args>
inline Wrapee *create_chained_pool_wrapper(Args &&... args) {
    auto pool = create_chained_pool_inside("pool", DefaultWorkerPoolSize);
    auto wrapper = new (pool) ConcreteWrapee(pool, std::forward<Args>(args)...);
    return wrapper;
}

}
