#include <tiny_printf.h>

#include <cstdlib>
#include <cstring>
#include <new>

#include "pool.h"
#include "platform.h"
#include "protobuf.h"

void *operator new(size_t size, fk::Pool &pool) {
    return pool.malloc(size);
}

namespace fk {

FK_DECLARE_LOGGER("pool");

// #define FK_LOGGING_POOL_VERBOSE

Pool::Pool(const char *name, size_t size, void *block) {
    name_ = name;
    block_ = block;
    ptr_ = block;
    size_ = size;
    remaining_ = size;

    #if defined(FK_LOGGING_POOL_VERBOSE)
    if (size_ > 0) {
        loginfo("create: 0x%p %s size=%zu ptr=0x%p (free=%" PRIu32 ")",
                this, name_, size_, ptr_, fk_free_memory());
    }
    #endif
}

void Pool::clear() {
    ptr_ = block_;
    remaining_ = size_;
    frozen_ = false;

    #if defined( FK_LOGGING_POOL_VERBOSE)
    loginfo("clear: 0x%p %s", this, name_);
    #endif
}

void *Pool::malloc(size_t size) {
    FK_ASSERT(!frozen_);

    auto aligned = aligned_size(size);

    #if defined(FK_LOGGING_POOL_VERBOSE)
    loginfo("malloc 0x%p %s size=%zu aligned=%zu (free=%zu)",
            this, name_, size, aligned, remaining_ - aligned);
    #endif

    FK_ASSERT(size_ >= aligned);
    FK_ASSERT(remaining_ >= aligned);

    auto *p = ptr_;
    ptr_ = ((uint8_t *)ptr_) + aligned;
    remaining_ -= aligned;

    return (void *)p;
}

void *Pool::copy(void *ptr, size_t size) {
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

uint8_t *Pool::encode(pb_msgdesc_t const *fields, void const *src, size_t *size) {
    size_t required = 0;
    if (!pb_get_encoded_size(&required, fields, src)) {
        return nullptr;
    }

    required += pb_varint_size(required);

    auto buffer = (uint8_t *)malloc(required);
    auto stream = pb_ostream_from_buffer(buffer, required);
    if (!pb_encode_delimited(&stream, fields, src)) {
        return nullptr;
    }

    FK_ASSERT(stream.bytes_written == required);

    if (size != nullptr) {
        *size = stream.bytes_written;
    }

    return buffer;
}

void *Pool::decode(pb_msgdesc_t const *fields, uint8_t *src, size_t size, size_t message_size) {
    auto ptr = malloc(message_size);
    auto stream = pb_istream_from_buffer(src, size);
    if (!pb_decode_delimited(&stream, fields, ptr)) {
        return nullptr;
    }

    return ptr;
}

Pool Pool::freeze(const char *name) {
    // TODO: Ideally this would keep track of children and warn about
    // allocations on them when we unfreeze.
    frozen_ = true;
    return Pool{ name, remaining_, ptr_ };
}

MallocPool *MallocPool::create(const char *name, size_t size) {
    auto memory = (uint8_t *)::malloc(size + sizeof(MallocPool));
    auto pooled = memory + sizeof(MallocPool);
    auto pool = new (memory) MallocPool(name, pooled, size);
    return pool;
}

}
