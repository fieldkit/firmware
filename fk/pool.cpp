#include <cstdlib>
#include <string.h>

#include "pool.h"
#include "platform.h"

namespace fk {

#define loginfo(f, ...)  loginfof("pool", f, ##__VA_ARGS__)

#define logerror(f, ...) logerrorf("pool", f, ##__VA_ARGS__)

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

Pool Pool::freeze(const char *name) {
    // TODO: Ideally this would keep track of children and warn about
    // allocations on them when we unfreeze.
    frozen_ = true;
    return Pool{ name, remaining_, ptr_ };
}

}
