#pragma once

#include "common.h"

namespace fk {

template<class T>
class circular_buffer {
private:
    T *buf_;
    const size_t max_size_;
    size_t head_{ 0 };
    size_t tail_{ 0 };
    bool full_{ false };

public:
    circular_buffer(T *buf, size_t size) : buf_(buf), max_size_(size) {
    }

public:
    struct iterator {
        size_t size_;
        T *buf_;
        size_t index_;

        iterator() : size_(0), buf_(nullptr), index_(0) {
        }

        iterator(size_t size, T *buf, size_t index) : size_(size), buf_(buf), index_(index) {
        }

        size_t index() const {
            return index_;
        }

        iterator operator++() {
            index_ = (index_ + 1) % size_;
            return *this;
        }

        bool operator!=(const iterator &other) const {
            return index_ != other.index_;
        }

        bool operator==(const iterator &other) const {
            return index_ == other.index_;
        }

        T &operator*() {
            return buf_[index_];
        }

        T &operator*() const {
            return buf_[index_];
        }
    };

public:
    bool sane_state() const {
        return head_ < max_size_ && tail_ < max_size_;
    }

    iterator begin() const {
        if (empty()) {
            return end();
        }
        return iterator{ max_size_, buf_, tail_ };
    }

    iterator end() const {
        return iterator{ max_size_, buf_, head_ };
    }

    iterator head() const {
        return iterator{ max_size_, buf_, head_ };
    }

    iterator tail() const {
        return iterator{ max_size_, buf_, tail_ };
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
            tail_ = i.index_;
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

    size_t size(iterator iter) const {
        if (iter == end()) {
            return 0;
        }
        if (head_ >= iter.index()) {
            return head_ - iter.index();
        }
        else {
            return max_size_ + head_ - iter.index();
        }
    }

    size_t size() const {
        auto size = max_size_;
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
