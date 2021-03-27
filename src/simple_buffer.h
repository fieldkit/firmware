#pragma once

#include "phylum.h"
#include "varint.h"

namespace phylum {

class simple_buffer {
private:
    uint8_t *ptr_{ nullptr };
    size_t size_{ 0 };
    size_t position_{ 0 };
    bool free_{ false };

public:
    simple_buffer() {
    }

    simple_buffer(simple_buffer &other) = delete;

    simple_buffer(simple_buffer &&other)
        : ptr_(exchange(other.ptr_, nullptr)), size_(other.size_), position_(other.position_),
          free_(exchange(other.free_, false)) {
    }

    explicit simple_buffer(size_t size) : ptr_((uint8_t *)malloc(size)), size_(size), free_(true) {
        assert(size > 0);
        clear();
    }

    explicit simple_buffer(uint8_t *ptr, size_t size) : ptr_(ptr), size_(size), position_(0), free_(false) {
        assert(size > 0);
    }

    explicit simple_buffer(uint8_t *ptr, size_t size, size_t position) : ptr_(ptr), size_(size), position_(position), free_(false) {
        assert(size > 0);
    }

    virtual ~simple_buffer() {
        if (free_ && ptr_ != nullptr) {
            free(ptr_);
            ptr_ = nullptr;
        }
    }

public:
    simple_buffer &operator=(simple_buffer const &other) {
        assert(!free_);
        ptr_ = other.ptr_;
        size_ = other.size_;
        position_ = other.position_;
        free_ = other.free_;
        return *this;
    }

    simple_buffer &operator=(simple_buffer &&other) {
        ptr_ = exchange(other.ptr_, nullptr);
        size_ = other.size_;
        position_ = other.position_;
        free_ = exchange(other.free_, false);
        return *this;
    }

public:
    size_t available() const {
        return size_ - position_;
    }

    uint8_t *ptr() const {
        return ptr_;
    }

    uint8_t *cursor(size_t offset = 0) const {
        assert(size_ > 0);
        assert(position_ + offset < size_);
        return ptr_ + position_ + offset;
    }

    size_t size() const {
        return size_;
    }

    size_t position() const {
        return position_;
    }

    void position(size_t position) {
        assert(position_ < size_);
        position_ = position;
    }

    size_t skip(size_t bytes) {
        assert(position_ + bytes <= size_);
        position_ += bytes;
        return position();
    }

    size_t skip_end() {
        position_ = size_;
        return size_;
    }

    template <typename T>
    int32_t fill(simple_buffer &sb, T flush) {
        return fill(sb.ptr(), sb.position(), flush);
    }

    template <typename T>
    int32_t fill(uint8_t const *source, size_t size, T flush) {
        auto copied = 0u;
        while (copied < size) {
            auto copying = std::min<file_size_t>(size - copied, size_ - position_);
            if (copying > 0) {
                memcpy(ptr_ + position_, source + copied, copying);
                position_ += copying;
                copied += copying;
            }
            else {
                auto err = flush(*this);
                if (err < 0) {
                    return err;
                }

                position_ = 0;
            }
        }
        return copied;
    }

    template <typename T>
    int32_t unsafe_all(T fn) {
        return fn(ptr_, size_);
    }

    template <typename T>
    int32_t unsafe_at(T fn) {
        return fn(ptr_ + position_, size_ - position_);
    }

    int32_t fill_from(simple_buffer &buffer) {
        auto copying = std::min<int32_t>(buffer.available(), available());
        if (copying > 0) {
            memcpy(cursor(), buffer.cursor(), copying);
            buffer.skip(copying);
        }
        return copying;
    }

    void clear(uint8_t value = 0xff) {
        memset(ptr_, value, size_);
        position_ = 0;
    }

    void rewind() {
        position_ = 0;
    }

    bool valid() {
        return ptr_ != nullptr && size_ > 0;
    }

    uint8_t *take(size_t size) {
        assert(size < size_ - position_);
        auto p = ptr_ + position_;
        position_ += size;
        return p;
    }

    bool room_for(size_t bytes) {
        return bytes + position_ <= size_;
    }

    simple_buffer begin_view() const {
        return simple_buffer(ptr_, size_);
    }

    simple_buffer end_view() const {
        return simple_buffer(nullptr, size_, size_);
    }

    int32_t constrain(size_t bytes) {
        size_ = position_ + bytes;
        return 0;
    }

    bool try_read(uint32_t &size) {
        if (position_ >= size_) {
            return false;
        }

        int32_t err = -1;
        size = (uint32_t)varint_decode(ptr_ + position_, size_ - position_, &err);
        if (err < 0) {
            return false;
        }

        position_ += varint_encoding_length(size);

        return true;
    }
};

} // namespace phylum
