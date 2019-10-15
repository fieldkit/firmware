#pragma once

#include "pool.h"

namespace fk {

template <class T>
struct pool_allocator {
    Pool *pool_{ nullptr };
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef T const *const_pointer;
    typedef T const &const_reference;

    // NOTE: This was added when trying to get nonstd::optional returns working.
    template<class U> struct rebind {
        typedef pool_allocator<U> other;
    };

    pool_allocator() : pool_(nullptr) {
    }

    pool_allocator(Pool &pool) : pool_(&pool) {
    }

    pool_allocator(Pool *pool) : pool_(pool) {
    }

    template<class U>
    pool_allocator(pool_allocator<U> const &pa) : pool_(pa.pool_) {
    }

    pointer allocate(size_t n) {
        FK_ASSERT(pool_ != nullptr);
        return pool_->malloc<T>(n);
    }

    void deallocate(pointer p, size_t n) {
        FK_ASSERT(pool_ != nullptr);
    }

    void construct(pointer p, T const &val) {
        FK_ASSERT(pool_ != nullptr);
        #if defined(FK_LOGGING_POOL_ALLOCATOR_VERBOSE)
        loginfo("(pa) new<val>(0x%p)", p);
        #endif
        new ((pointer)p) T(val);
    }

    void destroy(pointer p) {
        FK_ASSERT(pool_ != nullptr);
        #if defined(FK_LOGGING_POOL_ALLOCATOR_VERBOSE)
        loginfo("(pa) delete(0x%p)", p);
        #endif
        p->~T();
    }

    template<class... Args>
    void construct(pointer p, Args &&... args) {
        FK_ASSERT(pool_ != nullptr);
        #if defined(FK_LOGGING_POOL_ALLOCATOR_VERBOSE)
        loginfo("(pa) new<args>(0x%p)", p);
        #endif
        new ((pointer)p) T(std::forward<Args>(args)...);
    }
};

template<class T, class U>
constexpr bool operator==(pool_allocator<T> const &, pool_allocator<U> const &) {
    return true;
}

template<class T, class U>
constexpr bool operator!=(pool_allocator<T> const &, pool_allocator<U> const &) {
    return false;
}

}
