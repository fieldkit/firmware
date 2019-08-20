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
    Pool(const char *name, size_t size, void *block);

public:
    void *block() {
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
    StaticPool(const char *name) : Pool(name, aligned_size(N), (void *)data) {
    }

};

class MallocPool : public Pool {
public:
    MallocPool(const char *name, size_t size) : Pool(name, size, (void *)::malloc(size)) {
    }

    ~MallocPool() {
        free(block());
        block(nullptr, 0);
    }

};

class EmptyPool : public Pool {
public:
    EmptyPool() : Pool("empty", 0, nullptr) {
    }

};

#define __POOL_LINE_STR(x) #x
#define __POOL_LINE(x) __POOL_LINE_STR(x)
#define PoolHere(var, size) var(__FILE__ ":" __POOL_LINE(__LINE__), size)

template <class T>
struct pool_allocator {
    Pool *pool_;
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
        return pool_->malloc<T>(n);
    }

    void deallocate(pointer, size_t n) {
    }

    void construct(pointer p, T const &val) {
        new ((pointer)p) T(val);
    }

    void destroy(pointer p) {
        p->~T();
    }

    template<class... Args>
    void construct(pointer p, Args &&... args) {
        new ((pointer)p) T(std::forward<Args...>(args...));
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
