#pragma once

namespace fk {

struct BufferPtr {
    size_t size{ 0 };
    uint8_t *buffer{ nullptr };
    BufferPtr *link{ nullptr };

    BufferPtr() {
    }

    BufferPtr(size_t size, uint8_t *buffer) : size(size), buffer(buffer) {
    }

    BufferPtr(size_t size, uint8_t *buffer, BufferPtr *link) : size(size), buffer(buffer), link(link) {
    }

    operator bool() const {
        return size > 0;
    }
};

using EncodedMessage = BufferPtr;

} // namespace fk
