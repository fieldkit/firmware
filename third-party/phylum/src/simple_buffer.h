#pragma once

#include <functional>

#include "phylum.h"
#include "varint.h"

namespace phylum {

using free_fn_t = std::function<void(uint8_t const *)>;

template<typename PointerType = uint8_t>
class general_buffer {
private:
    PointerType *ptr_{ nullptr };
    size_t size_{ 0 };
    size_t position_{ 0 };
    free_fn_t free_;

public:
    general_buffer() {
    }

    general_buffer(general_buffer &other) = delete;

    general_buffer(general_buffer &&other)
        : ptr_(std::exchange(other.ptr_, nullptr)), size_(other.size_), position_(other.position_),
          free_(std::exchange(other.free_, free_fn_t{ nullptr })) {
    }

    explicit general_buffer(PointerType *ptr, size_t size, free_fn_t free) : ptr_(ptr), size_(size), position_(0), free_(free) {
        assert(size > 0);
        clear();
    }

    explicit general_buffer(PointerType *ptr, size_t size) : ptr_(ptr), size_(size), position_(0) {
    }

    explicit general_buffer(PointerType *ptr, size_t size, size_t position) : ptr_(ptr), size_(size), position_(position) {
        assert(size > 0);
        assert(position <= size);
    }

    virtual ~general_buffer() {
        if (free_ && ptr_ != nullptr) {
            free_(ptr_);
            ptr_ = nullptr;
        }
    }

public:
    general_buffer &operator=(general_buffer const &other) {
        assert(!free_);
        ptr_ = other.ptr_;
        size_ = other.size_;
        position_ = other.position_;
        free_ = other.free_;
        return *this;
    }

    general_buffer &operator=(general_buffer &&other) {
        ptr_ = std::exchange(other.ptr_, nullptr);
        size_ = other.size_;
        position_ = other.position_;
        free_ = std::exchange(other.free_, free_fn_t{ nullptr });
        return *this;
    }

public:
    PointerType *ptr() const {
        return ptr_;
    }

    void ptr(PointerType *ptr, size_t size) {
        assert((ptr_ != nullptr && ptr == nullptr) || (ptr_ == nullptr && ptr != nullptr));
        ptr_ = ptr;
        size_ = size;
    }

    size_t available() const {
        return size_ - position_;
    }

    PointerType *cursor(size_t offset = 0) const {
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
        assert(position_ <= size_);
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
    int32_t fill(general_buffer<uint8_t const> &sb, T flush) {
        return fill(sb.ptr() + sb.position(), sb.size() - sb.position(), flush);
    }

    template <typename T>
    int32_t fill(uint8_t const *source, size_t size, T flush) {
        auto copied = 0u;
        while (copied < size) {
            auto copying = std::min<size_t>(size - copied, size_ - position_);
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
    int32_t read_to_end(T fn) const {
        return fn(general_buffer<uint8_t const>(ptr_, size_));
    }

    template <typename T>
    int32_t read_to_position(T fn) const {
        return fn(general_buffer<uint8_t const>(ptr_, position_));
    }

    template <typename T>
    int32_t unsafe_all(T fn) {
        return fn(ptr_, size_);
    }

    template <typename T>
    int32_t unsafe_forever(T fn) {
        return fn(ptr_, size_);
    }

    template <typename T>
    int32_t unsafe_at(T fn) {
        return fn(ptr_ + position_, size_ - position_);
    }

    int32_t fill_from(general_buffer<uint8_t const> &buffer) {
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

    PointerType *take(size_t size) {
        assert(size <= size_ - position_);
        auto p = ptr_ + position_;
        position_ += size;
        return p;
    }

    bool room_for(size_t bytes) {
        return bytes + position_ <= size_;
    }

    general_buffer<uint8_t const> begin_view() const {
        return general_buffer<uint8_t const>(ptr_, size_, 0u);
    }

    general_buffer<uint8_t const> end_view() const {
        return general_buffer<uint8_t const>(nullptr, size_, size_);
    }

    int32_t constrain(size_t bytes) {
        size_ = bytes;
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

using simple_buffer = general_buffer<uint8_t>;
using write_buffer = general_buffer<uint8_t>;
using read_buffer = general_buffer<uint8_t const>;

} // namespace phylum
