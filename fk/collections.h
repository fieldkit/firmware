#pragma once

#include <algorithm>
#include <tl/expected.hpp>

#include "pool.h"
#include "exchange.h"

namespace fk {

template<typename T>
class collection {
private:
    struct item_t {
        T item;
        item_t *np{ nullptr };

        item_t() {
        }

        item_t(T &&item) : item(std::move(item)) {
        }
    };

    Pool *pool_{ nullptr };
    item_t *head_{ nullptr };

public:
    explicit collection() {
    }

    collection(Pool &pool) : pool_(&pool) {
    }

    collection(Pool *pool) : pool_(pool) {
    }

    collection(collection &&o) : pool_(exchange(o.pool_, nullptr)), head_(exchange(o.head_, nullptr)) {
    }

    explicit collection(collection const &o) : pool_(o.pool_), head_(o.head_) {
    }

public:
    void operator=(const collection&) = delete;

    collection &operator=(collection &&other) {
        pool_ = exchange(other.pool_, nullptr);
        head_ = exchange(other.head_, nullptr);
        return *this;
    }

public:
    class iterator {
    public:
        iterator(item_t *iter): iter_(iter) {
        }

    public:
        iterator operator++() {
            iter_ = iter_->np;
            return *this;
        }
        bool operator!=(const iterator &other) const {
            return iter_ != other.iter_;
        }

        T &operator*() const {
            return iter_->item;
        }

        T *operator->() const {
            return &iter_->item;
        }

    private:
        item_t *iter_;
    };

public:
    iterator begin() const {
        return iterator(head_);
    }

    iterator end() const {
        return iterator(nullptr);
    }

    void add(T value) {
        auto node = pool_->malloc_with<item_t>(std::move(value));
        append(node);
    }

    void add(collection<T> &other) {
        for (T &i : other) {
            add(i);
        }
    }

    template<class... Args>
    void emplace(Args&&... args) {
        auto node = pool_->malloc_with<item_t>(T(std::forward<Args>(args)...));
        append(node);
    }

    bool valid() const {
        return pool_ != nullptr;
    }

    size_t size() const {
        size_t i = 0;
        for (auto iter = head_; iter != nullptr; iter = iter->np, i++) { }
        return i;
    }

private:
    void append(item_t *node) {
        if (head_ != nullptr) {
            for (auto iter = head_; ; iter = iter->np) {
                if (iter->np == nullptr) {
                    iter->np = node;
                    break;
                }
            }
        }
        else {
            head_ = node;
        }
    }

};

}
