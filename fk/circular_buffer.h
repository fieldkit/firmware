#pragma once

#include "common.h"

namespace fk {

template<class T>
class circular_buffer {
private:
    T *buf_;
    const size_t max_size_;
    size_t head_ = 0;
    size_t tail_ = 0;
    bool full_ = 0;

public:
    circular_buffer(T *buf, size_t size) : buf_(buf), max_size_(size) {
    }

public:
    struct iterator {
        size_t size;
        T *buf;
        size_t head;
        size_t index;

        iterator(size_t size, T *buf, size_t head, size_t index) : size(size), buf(buf), head(head), index(index) {
        }

        iterator operator++() {
            index = (index + 1) % size;
            if (index == head) {
                index = size;
            }
            return *this;
        }

        bool operator!=(const iterator &other) const {
            return index != other.index;
        }

        bool operator==(const iterator &other) const {
            return index == other.index;
        }

        T &operator*() {
            return buf[index];
        }

        T &operator*() const {
            return buf[index];
        }
    };

public:
    iterator begin() {
        if (empty()) {
            return end();
        }
        return iterator{ max_size_, buf_, head_, tail_ };
    }

    iterator end() {
        return iterator{ 0, 0, head_, max_size_ };
    }

    iterator head() {
        return iterator{ 0, 0, max_size_, head_ };
    }

    iterator tail() {
        return iterator{ 0, 0, max_size_, tail_ };
    }

    void zero() {
        bzero(buf_, max_size_);
        head_ = 0;
        tail_ = 0;
        full_ = false;
    }

    void skip(iterator i) {
        if (i == end()) {
            head_ = tail_ = 0;
        }
        else {
            tail_ = i.index;
        }
        full_ = false;
    }

    void append(T const item) {
        buf_[head_] = item;

        if (full_) {
            tail_ = (tail_ + 1) % max_size_;
        }

        head_ = (head_ + 1) % max_size_;
        full_ = head_ == tail_;
    }

    T peek_head() {
        if (empty()) {
            return T();
        }
        return buf_[head_];
    }

    T peek_tail() {
        if (empty()) {
            return T();
        }
        return buf_[tail_];
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
