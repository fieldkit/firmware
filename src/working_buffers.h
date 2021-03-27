#pragma once

#include <simple_buffer.h>

namespace phylum {

class buffers;

class loaned_buffer : public simple_buffer {
private:
    buffers &buffers_;
    uint8_t *ptr_{ nullptr };
    size_t size_{ 0 };

public:
    loaned_buffer(buffers &buffers, uint8_t *ptr, size_t size) : buffers_(buffers), ptr_(ptr), size_(size) {
    }

    virtual ~loaned_buffer() {
    }

public:

};

class buffers {
private:

public:
    simple_buffer allocate(size_t size) {
        return simple_buffer{ size };
    }

};

} // namespace phylum
