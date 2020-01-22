#include <tiny_printf.h>

#include <cstdlib>
#include <cstring>
#include <new>

#include "pool.h"
#include "platform.h"
#include "protobuf.h"
#include "config.h"
#include "memory.h"

void *operator new(size_t size, fk::Pool &pool) {
    return pool.malloc(size);
}

void *operator new(size_t size, fk::Pool *pool) {
    return pool->malloc(size);
}

namespace fk {

FK_DECLARE_LOGGER("pool");

// #define FK_LOGGING_POOL_VERBOSE
// #define FK_LOGGING_POOL_MALLOC_FREE

Pool::Pool(const char *name, size_t size, void *block, size_t taken) {
    name_ = name;
    block_ = block;
    ptr_ = ((uint8_t *)block) + taken;
    size_ = size;
    remaining_ = size - taken;

    #if defined(FK_LOGGING_POOL_VERBOSE)
    if (size_ > 0) {
        loginfo("create: 0x%p %s size=%zu ptr=0x%p", this, name_, size_, ptr_);
    }
    #endif
}

Pool::~Pool() {
}

void Pool::clear() {
    ptr_ = block_;
    remaining_ = size_;

    #if defined( FK_LOGGING_POOL_VERBOSE)
    loginfo("clear: 0x%p %s", this, name_);
    #endif
}

void *Pool::malloc(size_t allocating) {
    FK_ASSERT(allocating > 0);

    auto aligned = aligned_size(allocating);

    #if defined(FK_LOGGING_POOL_VERBOSE)
    loginfo("malloc 0x%p %s size=%zu allocating=%zu/%zu remaining=%zu remaining-aligned=%zu",
            this, name_, size_, allocating, aligned, remaining_, remaining_ - aligned);
    #endif

    FK_ASSERT(block_ != nullptr);
    FK_ASSERT(size_ > 0);
    FK_ASSERT(remaining_ > 0);
    FK_ASSERT(size_ >= aligned);
    FK_ASSERT(remaining_ >= aligned);

    auto *p = ptr_;
    ptr_ = ((uint8_t *)ptr_) + aligned;
    remaining_ -= aligned;

    return (void *)p;
}

void *Pool::copy(void const *ptr, size_t size) {
    void *newPtr = malloc(size);
    memcpy(newPtr, ptr, size);
    return newPtr;
}

char *Pool::strdup(const char *str) {
    if (str == nullptr) {
        return nullptr;
    }

    auto length = strlen(str);
    auto ptr = (char *)malloc(length + 1);
    strncpy(ptr, str, length + 1);
    return ptr;
}

char *Pool::strndup(const char *str, size_t length) {
    if (str == nullptr) {
        return nullptr;
    }

    auto ptr = (char *)malloc(length + 1);
    strncpy(ptr, str, length + 1);
    ptr[length] = 0;
    return ptr;
}

char *Pool::sprintf(const char *str, ...) {
    va_list args;
    va_start(args, str);
    auto req = tiny_vsnprintf(nullptr, 0, str, args);
    va_end(args);

    auto ptr = (char *)malloc(req + 1);

    va_start(args, str);
    tiny_vsnprintf(ptr, req + 1, str, args);
    va_end(args);

    ptr[req] = 0;
    return ptr;
}

EncodedMessage *Pool::encode(pb_msgdesc_t const *fields, void const *src, bool delimited) {
    size_t required = 0;
    if (!pb_get_encoded_size(&required, fields, src)) {
        return nullptr;
    }

    required += delimited ? pb_varint_size(required) : 0;

    auto buffer = (uint8_t *)malloc(required);
    auto stream = pb_ostream_from_buffer(buffer, required);
    if (delimited) {
        if (!pb_encode_delimited(&stream, fields, src)) {
            return nullptr;
        }
    }
    else {
        if (!pb_encode(&stream, fields, src)) {
            return nullptr;
        }
    }

    FK_ASSERT(stream.bytes_written == required);

    return new (*this) EncodedMessage(stream.bytes_written, buffer);
}

void *Pool::decode(pb_msgdesc_t const *fields, uint8_t *src, size_t size, size_t message_size) {
    auto ptr = malloc(message_size);
    auto stream = pb_istream_from_buffer(src, size);
    if (!pb_decode_delimited(&stream, fields, ptr)) {
        return nullptr;
    }

    return ptr;
}

class InsidePool : public Pool {
public:
    InsidePool(const char *name, void *ptr, size_t size, size_t taken) : Pool(name, size, ptr, taken) {
    }

    virtual ~InsidePool() {
    }

public:
    static void operator delete(void *p) {
        fk_standard_page_free(p);
    }

};

Pool *create_pool_inside(const char *name) {
    auto size = StandardPageSize;
    auto ptr = fk_standard_page_malloc(size, name);
    auto overhead = sizeof(InsidePool);
    return new (ptr) InsidePool(name, ptr, size, overhead);
}

StandardPool::StandardPool(const char *name) : Pool(name, StandardPageSize, (void *)fk_standard_page_malloc(StandardPageSize, name), 0), free_self_{ true } {
}

StandardPool::StandardPool(const char *name, void *ptr, size_t size, size_t taken) : Pool(name, size, ptr, taken), free_self_{ false } {
}

StandardPool::~StandardPool() {
    #if defined(FK_LOGGING_POOL_MALLOC_FREE)
    loginfo("free: 0x%p %s size=%zu ptr=0x%p (free=%" PRIu32 ")",
            this, name(), size(), block(), fk_free_memory());
    #endif
    if (sibling_ != nullptr) {
        delete sibling_;
        sibling_ = nullptr;
    }
    if (free_self_) {
        auto ptr = block();
        if (ptr != nullptr) {
            block(nullptr, 0);
            fk_standard_page_free(ptr);
        }
    }
}

void *StandardPool::malloc(size_t bytes) {
    FK_ASSERT(bytes > 0);

    if (can_malloc(bytes)) {
        return Pool::malloc(bytes);
    }

    if (sibling_ == nullptr) {
        auto ptr = fk_standard_page_malloc(size(), name());
        auto overhead = sizeof(StandardPool);
        sibling_ = new (ptr) StandardPool(name(), ptr, size(), overhead);
    }

    return sibling_->malloc(bytes);
}

Pool *create_standard_pool_inside(const char *name) {
    auto size = StandardPageSize;
    auto ptr = fk_standard_page_malloc(size, name);
    auto overhead = sizeof(StandardPool);
    return new (ptr) StandardPool(name, ptr, size, overhead);
}

}
