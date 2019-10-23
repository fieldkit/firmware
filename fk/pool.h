#pragma once

#include <cinttypes>
#include <cstdlib>
#include <type_traits>
#include <utility>

#include <pb_encode.h>
#include <pb_decode.h>

#include "common.h"
#include "encoded_message.h"

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
        return used();
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
    void *copy(void const *ptr, size_t size);
    char *strdup(const char *str);
    char *strndup(const char *str, size_t len);
    char *sprintf(const char *str, ...);
    Pool freeze(const char *name);
    EncodedMessage *encode(pb_msgdesc_t const *fields, void const *src, bool delimited = true);
    void *decode(pb_msgdesc_t const *fields, uint8_t *src, size_t size, size_t message_size);

    template<typename T>
    T *malloc() {
        return (T *)malloc(sizeof(T));
    }

    template<typename T>
    T *malloc_with(T &&value) {
        auto ptr = (T *)malloc(sizeof(T));
        *ptr = std::move(value);
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

Pool *create_pool_inside(const char *name, size_t size);

/*
#define __POOL_LINE_STR(x) #x
#define __POOL_LINE(x) __POOL_LINE_STR(x)
#define PoolHere(var, size) var(__FILE__ ":" __POOL_LINE(__LINE__), size)
#define StaticPoolHere(size)  StaticPool<size>(__FILE__ ":" __POOL_LINE(__LINE__))
*/

}

/**
 * New operator that allocates from a memory pool. Note that this is global
 * because the compiler can't really tell which new or delete you're referring
 * to in all scenarios. Taking a Pool makes things safe, though.
 */
void *operator new(size_t size, fk::Pool &pool);

/**
 * New operator that allocates from a memory pool. Note that this is global
 * because the compiler can't really tell which new or delete you're referring
 * to in all scenarios. Taking a Pool makes things safe, though.
 */
void *operator new(size_t size, fk::Pool *pool);
