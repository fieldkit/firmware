#pragma once

#include "memory.h"
#include "config.h"

namespace fk {

class StandardPage {
private:
    void *ptr_;

public:
    void *ptr() const {
        return ptr_;
    }

    size_t size() const {
        return StandardPageSize;
    }

public:
    StandardPage() : ptr_(fk_standard_page_malloc(StandardPageSize)) {
    }

    virtual ~StandardPage() {
        if (ptr_ != nullptr) {
            fk_standard_page_free(ptr_);
            ptr_ = nullptr;
        }
    }
};

}
