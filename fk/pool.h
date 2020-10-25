#pragma once

#include <cinttypes>
#include <cstdlib>
#include <type_traits>
#include <utility>

#include <pb_encode.h>
#include <pb_decode.h>

#include "common.h"
#include "encoded_message.h"
#include "memory.h"

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
    size_t taken_;

public:
    explicit Pool(const char *name, size_t size, void *block, size_t taken);
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

    void clear();
    void *copy(void const *ptr, size_t size);
    char *strdup(const char *str);
    char *strndup(const char *str, size_t len);
    char *sprintf(const char *str, ...);
    EncodedMessage *encode(pb_msgdesc_t const *fields, void const *src, bool delimited = true);
    void *decode(pb_msgdesc_t const *fields, uint8_t *src, size_t size, size_t message_size);
    EncodedMessage *copy(EncodedMessage const *message);
    EncodedMessage *wrap(uint8_t *buffer, size_t size);

public:
    virtual size_t allocated() const {
        return used();
    }

    virtual size_t size() const {
        return size_;
    }

    virtual size_t used() const {
        return size_ - remaining_;
    }

    virtual void *malloc(size_t size);

    void *calloc(size_t size) {
        auto ptr = malloc(size);
        bzero(ptr, size);
        return ptr;
    }

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

class StandardPool : public Pool {
private:
    bool free_self_{ false };
    StandardPool *sibling_{ nullptr };

public:
    explicit StandardPool(const char *name);
    explicit StandardPool(const char *name, void *ptr, size_t size, size_t taken);
    virtual ~StandardPool();

public:
    static void operator delete(void *p) {
        fk_standard_page_free(p);
    }

public:
    size_t allocated() const override {
        return Pool::allocated() + (sibling_ == nullptr ? 0u : sibling_->allocated());
    }

    size_t size() const override {
        return Pool::size() + (sibling_ == nullptr ? 0u : sibling_->size());
    }

    size_t used() const override {
        return Pool::used() + (sibling_ == nullptr ? 0u : sibling_->used());
    }

    void *malloc(size_t bytes) override;

    bool can_malloc(size_t bytes) const {
        return (Pool::size() - Pool::used()) >= aligned_size(bytes);
    }

};

Pool *create_pool_inside(const char *name);

Pool *create_standard_pool_inside(const char *name);

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
void *operator new[](size_t size, fk::Pool &pool);

/**
 * New operator that allocates from a memory pool. Note that this is global
 * because the compiler can't really tell which new or delete you're referring
 * to in all scenarios. Taking a Pool makes things safe, though.
 */
void *operator new(size_t size, fk::Pool *pool);

/**
 * New operator that allocates from a memory pool. Note that this is global
 * because the compiler can't really tell which new or delete you're referring
 * to in all scenarios. Taking a Pool makes things safe, though.
 */
void *operator new[](size_t size, fk::Pool *pool);
