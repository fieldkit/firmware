#pragma once

#include <algorithm>
#include <tl/expected.hpp>

#include "pool.h"

namespace fk {

template<class T, class U = T>
T exchange(T& obj, U&& new_value) {
    T old_value = std::move(obj);
    obj = std::forward<U>(new_value);
    return old_value;
}

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
    void operator=(const collection&) = delete;

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
    class iterator {
    public:
        iterator(item_t *iter): iter_(iter) {
        }
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
    private:
        item_t *iter_;
    };

    collection &operator=(collection &&other) {
        pool_ = exchange(other.pool_, nullptr);
        head_ = exchange(other.head_, nullptr);
        return *this;
    }

public:
    void add(T value) {
        auto node = pool_->malloc_with<item_t>(std::move(value));
        append(node);
    }

    template<class... Args>
    void emplace(Args&&... args) {
        auto node = pool_->malloc_with<item_t>(T(std::forward<Args>(args)...));
        append(node);
    }

    iterator begin() const {
        return iterator(head_);
    }

    iterator end() const {
        return iterator(nullptr);
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
