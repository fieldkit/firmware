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
    static void operator delete(void *p) {
        FK_ASSERT(!!"PoolPointer::delete should never be called");
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
class WeakPoolWrapper : public PoolPointer<Wrapped> {
private:
    Pool *pool_;
    ConcreteWrapped wrapped_;

public:
    WeakPoolWrapper(Pool &pool, Args&&... args) : pool_(&pool), wrapped_(&pool, std::forward<Args>(args)...) {
    }

    virtual ~WeakPoolWrapper() {
        SEGGER_RTT_WriteString(0, "::dtor\n");
    }

public:
    void operator delete(void *p) {
        // We are freed automatically when the pool is deleted in our
        // destructor. So we don't need to do anything in here.
        SEGGER_RTT_WriteString(0, "::delete\n");
    }

public:
    Pool *pool() override {
        return pool_;
    }

    Wrapped *get() override {
        return &wrapped_;
    }

};

template<typename Wrapped, typename ConcreteWrapped = Wrapped, class... Args>
class StandardPoolWrapper : public PoolPointer<Wrapped> {
private:
    Pool *pool_;
    ConcreteWrapped wrapped_;

public:
    StandardPoolWrapper(Pool *pool, Args&&... args) : pool_(pool), wrapped_(pool, std::forward<Args>(args)...) {
    }

    virtual ~StandardPoolWrapper() {
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

template<typename Wrapped, typename Wrapee = PoolPointer<Wrapped>, typename ConcreteWrapped = Wrapped, typename ConcreteWrapee = StandardPoolWrapper<Wrapped, ConcreteWrapped>, class... Args>
inline Wrapee *create_chained_pool_wrapper(Args &&... args) {
    auto pool = create_standard_pool_inside(TypeName<Wrapped>::get());
    return new (pool) ConcreteWrapee(pool, std::forward<Args>(args)...);
}

}
