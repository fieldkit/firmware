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

    Pool *pool() const {
        return pool_;
    }

public:
    class iterator {
    public:
        iterator(item_t *iter) : iter_(iter) {
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

    template<typename SortKeyFn>
    bool sort(SortKeyFn key_fn) {
        auto modified = false;
        if (head_ == nullptr) {
            return modified;
        }

        auto sorted = false;
        auto l = head_;
        auto r = (item_t *)nullptr;
        do {
            sorted = true;
            l = head_;

            while (l->np != r) {
                auto lkey = key_fn(l->item);
                auto rkey = key_fn(l->np->item);

                if (lkey > rkey) {
                    auto temp = l->item;
                    l->item = l->np->item;
                    l->np->item = temp;
                    sorted = false;
                    modified = true;
                }

                l = l->np;
            }

            r = l;
        }
        while (!sorted);

        return modified;
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

template<typename K, size_t TableSize = 10>
struct hash_key {
    uint32_t operator()(K const &key) const {
        return reinterpret_cast<uint32_t>(key) % TableSize;
    }
};

template<typename K, typename V>
class hash_node {
private:
    K key_;
    V value_;
    hash_node *np_;

public:
    hash_node(K const &key, V const &value) : key_(key), value_(value), np_(nullptr) {
    }

public:
    K key() const {
        return key_;
    }

    V value() const {
        return value_;
    }

    void value(V value) {
        value_ = value;
    }

    hash_node *np() const {
        return np_;
    }

    void np(hash_node *np) {
        np_ = np;
    }
};

/**
 * https://medium.com/@aozturk/simple-hash-map-hash-table-implementation-in-c-931965904250
 */
template<typename K, typename V, size_t TableSize = 10, typename F = hash_key<K, TableSize>>
class hash_map {
private:
    Pool *pool_{ nullptr };
    hash_node<K, V> **table_{ nullptr };
    F hash_func_;

public:
    explicit hash_map() {
    }

    explicit hash_map(Pool &pool) : pool_(&pool), table_(new (*pool_) hash_node<K, V> *[TableSize]()) {
    }

    explicit hash_map(Pool *pool) : pool_(pool), table_(new (*pool_) hash_node<K, V> *[TableSize]()) {
    }

    explicit hash_map(hash_map &&o) : pool_(exchange(o.pool_, nullptr)), table_(exchange(o.table_, nullptr)), hash_func_(o.hash_func_) {
    }

    // cppcheck-suppress copyCtorPointerCopying
    explicit hash_map(hash_map const &o) : pool_(o.pool_), table_(o.table_) {
    }

    virtual ~hash_map() {
    }

public:
    void operator=(const hash_map&) = delete;

    hash_map &operator=(hash_map &&other) {
        pool_ = exchange(other.pool_, nullptr);
        table_ = exchange(other.table_, nullptr);
        return *this;
    }

public:
    bool get(K const &key, V &value) {
        auto hash_value = hash_func_(key);
        auto entry = table_[hash_value];
        while (entry != nullptr) {
            if (entry->key() == key) {
                value = entry->value();
                return true;
            }
            entry = entry->np();
        }
        return false;
    }

    void put(K const &key, V const &value) {
        auto hash_value = hash_func_(key);
        hash_node<K, V> *prev = nullptr;
        hash_node<K, V> *entry = table_[hash_value];

        while (entry != nullptr && entry->key() != key) {
            prev = entry;
            entry = entry->np();
        }

        if (entry == nullptr) {
            FK_ASSERT(pool_ != nullptr);
            entry = new (pool_) hash_node<K, V>(key, value);
            if (prev == nullptr) {
                table_[hash_value] = entry;
            }
            else {
                prev->np(entry);
            }
        }
        else {
            entry->value(value);
        }
    }

    void remove(K const &key) {
        auto hash_value = hash_func_(key);
        hash_node<K, V> *prev = nullptr;
        hash_node<K, V> *entry = table_[hash_value];

        while (entry != nullptr && entry->key() != key) {
            prev = entry;
            entry = entry->np();
        }

        if (entry == nullptr) {
            return;
        }

        if (prev == nullptr) {
            table_[hash_value] = entry->np();
        }
        else {
            prev->np(entry->np());
        }
    }
};

} // namespace fk
