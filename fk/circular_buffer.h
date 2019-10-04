#pragma once

#include "common.h"

namespace fk {

template<class T>
class circular_buffer {
private:
    T *buf_;
    size_t head_ = 0;
    size_t tail_ = 0;
    const size_t max_size_;
    bool full_ = 0;

public:
    circular_buffer(T *buf, size_t size) : buf_(buf), max_size_(size) {
    }

public:
    void append(T item) {
        buf_[head_] = item;

        if (full_) {
            tail_ = (tail_ + 1) % max_size_;
        }

        head_ = (head_ + 1) % max_size_;
        full_ = head_ == tail_;
    }

    T get() {
        if (empty()) {
            return T();
        }
        auto val = buf_[tail_];
        full_ = false;
        tail_ = (tail_ + 1) % max_size_;
        return val;
    }

    void clear() {
        head_ = tail_;
        full_ = false;
    }

    bool empty() const {
        return (!full_ && (head_ == tail_));
    }

    bool full() const {
        return full_;
    }

    size_t capacity() const {
        return max_size_;
    }

    size_t size() const {
        size_t size = max_size_;

        if (!full_) {
            if (head_ >= tail_) {
                size = head_ - tail_;
            }
            else {
                size = max_size_ + head_ - tail_;
            }
        }
        return size;
    }

    T *buffer() {
        return buf_;
    }

};

}
