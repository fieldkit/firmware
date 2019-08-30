#pragma once

#include <cinttypes>
#include <cstdlib>
#include <type_traits>
#include <utility>

#include <pb_encode.h>
#include <pb_decode.h>

#include "common.h"

namespace fk {

constexpr size_t AlignedOn = sizeof(uint32_t);

constexpr size_t aligned_size(const size_t size) {
    return (size % AlignedOn != 0) ? (size + (AlignedOn - (size % AlignedOn))) : size;
}

class Pool {
private:
    const char *name_;
    void *block_;
    void *ptr_;
    size_t remaining_;
    size_t size_;
    bool frozen_{ false };

public:
    Pool(const char *name, size_t size, void *block, size_t taken);
    virtual ~Pool();

public:
    const char *name() const {
        return name_;
    }

    void *block() const {
        return block_;
    }

    void block(void *block, size_t size) {
        block_ = block;
        ptr_ = block;
        size_ = size;
        remaining_ = size;
    }

    size_t allocated() const {
        return size_ - remaining_;
    }

    size_t size() const {
        return size_;
    }

    size_t used() const {
        return size_ - remaining_;
    }

    bool frozen() const {
        return frozen_;
    }

    void clear();
    void *malloc(size_t size);
    void *copy(void *ptr, size_t size);
    char *strdup(const char *str);
    char *strndup(const char *str, size_t len);
    char *sprintf(const char *str, ...);
    Pool freeze(const char *name);
    uint8_t *encode(pb_msgdesc_t const *fields, void const *src, size_t *size);
    void *decode(pb_msgdesc_t const *fields, uint8_t *src, size_t size, size_t message_size);

    template<typename T>
    T *malloc() {
        return (T *)malloc(sizeof(T));
    }

    template<typename T>
    T *malloc_with(T &&value) {
        auto ptr = (T *)malloc(sizeof(T));
        *ptr = value;
        return ptr;
    }

    template<typename T, size_t N>
    T *malloc_with(T (&&value)[N]) {
        auto ptr = (T *)malloc(sizeof(T) * N);
        memcpy(ptr, &value, sizeof(T) * N);
        return ptr;
    }

    template<typename T>
    T *malloc(size_t n) {
        return (T *)malloc(sizeof(T) * n);
    }

};

template<size_t N>
class StaticPool : public Pool {
private:
    alignas(sizeof(uint32_t)) typename std::aligned_storage<sizeof(uint8_t), alignof(uint8_t)>::type data[aligned_size(N)];

public:
    StaticPool(const char *name) : Pool(name, aligned_size(N), (void *)data, 0) {
    }

};

class MallocPool : public Pool {
public:
    MallocPool(const char *name, size_t size);
    MallocPool(const char *name, void *ptr, size_t size, size_t taken);
    virtual ~MallocPool();

};

class EmptyPool : public Pool {
public:
    EmptyPool() : Pool("empty", 0, nullptr, 0) {
    }

};

template<typename T>
class PoolWrapper {
private:
    MallocPool pool_;
    T *wrapped_;

public:
    PoolWrapper(uint8_t *p, size_t size, size_t taken) :
        pool_(__PRETTY_FUNCTION__, p, size, taken) {
    }

    template<class... Args>
    T *create(Args &&... args) {
        wrapped_ = new (pool_) T(pool_, std::forward<Args>(args)...);
        return wrapped_;
    }

    virtual ~PoolWrapper() {
        pool_.block(nullptr, 0);
    }

public:
    void operator delete(void *p) {
        fk_free(p);
    }

public:
    MallocPool &pool() {
        return pool_;
    }

    T *get() {
        return wrapped_;
    }

};

template<typename T, size_t Size, typename W = PoolWrapper<T>, class... Args>
inline W *create_pool_wrapper(Args &&... args) {
    auto block = (uint8_t *)fk_malloc(Size);

    auto wrapper_size = aligned_size(sizeof(W));
    auto overhead = wrapper_size;
    auto pool_memory = block + overhead;

    auto wrapper = new ((W *)(block)) PoolWrapper<T>(pool_memory, Size - overhead, 0);

    wrapper->create(std::forward<Args>(args)...);

    return wrapper;
}

#define __POOL_LINE_STR(x) #x
#define __POOL_LINE(x) __POOL_LINE_STR(x)
#define PoolHere(var, size) var(__FILE__ ":" __POOL_LINE(__LINE__), size)
#define StaticPoolHere(size)  StaticPool<size>(__FILE__ ":" __POOL_LINE(__LINE__))

// #define FK_LOGGING_POOL_ALLOCATOR_VERBOSE

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

/**
 * New operator that allocates from a memory pool. Note that this is global
 * because the compiler can't really tell which new or delete you're referring
 * to in all scenarios. Taking a Pool makes things safe, though.
 */
void *operator new(size_t size, fk::Pool &pool);
