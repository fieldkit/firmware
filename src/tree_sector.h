#pragma once

#include "sector_map.h"
#include "sector_allocator.h"
#include "delimited_buffer.h"

namespace phylum {

class Keys {
public:
    // Returns the position where 'key' should be inserted in a leaf node
    // that has the given keys.
    // NOTE: These and the following methods do a simple linear search, which is
    // just fine for N or M < 100. Any large and a Binary Search is better.
    template <typename KEY, typename NODE> static inline index_type leaf_position_for(const KEY &key, const NODE &node) {
        index_type k = 0;
        while ((k < node.number_keys) && (node.keys[k] < key)) {
            ++k;
        }
        assert(k <= node.number_keys);
        return k;
    }

    // Returns the position where 'key' should be inserted in an inner node
    // that has the given keys.
    template <typename KEY, typename NODE> static inline index_type inner_position_for(const KEY &key, const NODE &node) {
        index_type k = 0;
        while ((k < node.number_keys) && ((node.keys[k] < key) || (node.keys[k] == key))) {
            ++k;
        }
        return k;
    }
};

class heap_buffer_allocator {
public:
    simple_buffer allocate(size_t size) {
        return simple_buffer{ size };
    }

};

template <typename KEY, typename VALUE, size_t InnerSize, size_t LeafSize, typename BufferAllocatorType = heap_buffer_allocator>
class tree_sector {
private:
    using default_node_type = tree_node_t<KEY, VALUE, InnerSize, LeafSize>;

    static constexpr size_t ScopeNameLength = 32;

    struct insertion_t {
        bool split{ false };
        uint32_t key{ 0 };
        node_ptr_t left;
        node_ptr_t right;
    };

    struct persisted_node_t {
        default_node_type *node{ nullptr };
        node_ptr_t ptr{};

        persisted_node_t() {
        }

        persisted_node_t(default_node_type *node, node_ptr_t ptr) : node(node), ptr(ptr) {
        }
    };

private:
    BufferAllocatorType buffer_allocator_;
    sector_map *sectors_;
    sector_allocator *allocator_;
    delimited_buffer buffer_;
    dhara_sector_t sector_{ InvalidSector };
    dhara_sector_t root_{ InvalidSector };
    bool dirty_{ false };
    const char *prefix_{ "tree-sector" };
    char name_[ScopeNameLength];

public:
    tree_sector(sector_map &sectors, sector_allocator &allocator, simple_buffer &&buffer, dhara_sector_t root,
                const char *prefix)
        : sectors_(&sectors), allocator_(&allocator), buffer_(std::move(buffer)), root_(root), prefix_(prefix) {
        name("%s[%d]", prefix_, root_);
    }

    tree_sector(tree_sector &other, dhara_sector_t root, const char *prefix)
        : sectors_(other.sectors_), allocator_(other.allocator_), buffer_(other.sector_size()), root_(root),
          prefix_(prefix) {
        name("%s[%d]", prefix_, root_);
    }

    virtual ~tree_sector() {
    }

public:
    const char *name() const {
        return name_;
    }

    dhara_sector_t sector() const {
        return sector_;
    }

    void dirty(bool value) {
        dirty_ = value;
        if (value) {
            phydebugf("%s dirty", name());
        }
    }

    bool dirty() {
        return dirty_;
    }

protected:
    size_t sector_size() const {
        return buffer_.size();
    }

    delimited_buffer &db() {
        return buffer_;
    }

    sector_allocator &allocator() {
        return *allocator_;
    }

    // Allow users to call this to set the sector and defer the read
    // to when the code actually needs to do a read.
    void sector(dhara_sector_t sector) {
        sector_ = sector;
        name("%s[%d]", prefix_, sector_);
    }

private:
    static persisted_node_t find_sector_root(dhara_sector_t sector, delimited_buffer &db) {
        persisted_node_t selected;
        for (auto iter = db.begin(); iter != db.end(); ++iter) {
            auto node = iter->as<default_node_type>();
            if (selected.node == nullptr || selected.node->depth < node->depth) {
                selected = persisted_node_t{ node, node_ptr_t{ sector, (sector_offset_t)iter->position } };
            }
        }
        return selected;
    }

    static persisted_node_t find_node_in_sector(delimited_buffer &db, node_ptr_t ptr) {
        persisted_node_t selected;
        for (auto iter = db.begin(); iter != db.end(); ++iter) {
            auto node = iter->as<default_node_type>();
            if (iter->position == ptr.position) {
                phydebugf("find-node-in-sector: %d:%d (%d) !", ptr.sector, ptr.position, iter->position);
                return persisted_node_t{ node, ptr };
            } else {
                phydebugf("find-node-in-sector: %d:%d (%d)", ptr.sector, ptr.position, iter->position);
            }
        }
        phydebugf("find-node-in-sector: eos");
        return persisted_node_t{};
    }

private:
    void name(const char *f, ...) {
        va_list args;
        va_start(args, f);
        phy_vsnprintf(name_, sizeof(name_), f, args);
        va_end(args);
    }

    int32_t leaf_insert_nonfull(default_node_type *node, uint32_t &key, uint32_t &value, unsigned index) {
        assert(node->type == node_type::Leaf);
        assert(node->number_keys < InnerSize);
        assert(index < InnerSize);
        assert(index <= node->number_keys);

        if (node->keys[index] == key) {
            // We are inserting a duplicate value. Simply overwrite the old one
            node->d.values[index] = value;
        } else {
            for (auto i = node->number_keys; i > index; --i) {
                node->keys[i] = node->keys[i - 1];
                node->d.values[i] = node->d.values[i - 1];
            }
            node->number_keys++;
            node->keys[index] = key;
            node->d.values[index] = value;
        }

        phydebugf("%s value node", name());

        dirty(true);

        return 0;
    }

    int32_t leaf_node_insert(default_node_type *node, node_ptr_t node_ptr, uint32_t key, uint32_t value,
                             insertion_t &insertion) {
        logged_task lt{ "leaf-node" };

        auto index = Keys::leaf_position_for(key, *node);

        if (node->number_keys >= LeafSize) {
            phydebugf("node full, splitting");

            node_ptr_t sibling_ptr;
            return allocate_node(sibling_ptr, [&](default_node_type *new_sibling, node_ptr_t ignored_ptr) {
                auto threshold = (LeafSize + 1) / 2;
                new_sibling->type = node_type::Leaf;
                new_sibling->number_keys = node->number_keys - threshold;
                for (auto j = 0u; j < new_sibling->number_keys; ++j) {
                    new_sibling->keys[j] = node->keys[threshold + j];
                    new_sibling->d.values[j] = node->d.values[threshold + j];
                }

                node->number_keys = threshold;
                dirty(true); // Reorder?

                if (index < threshold) {
                    auto err = leaf_insert_nonfull(node, key, value, index);
                    if (err < 0) {
                        return err;
                    }
                } else {
                    auto err = leaf_insert_nonfull(new_sibling, key, value, index - threshold);
                    if (err < 0) {
                        return err;
                    }
                }

                insertion.split = true;
                insertion.key = new_sibling->keys[0];
                insertion.left = node_ptr;
                insertion.right = sibling_ptr;

                return 0;
            });
        } else {
            return leaf_insert_nonfull(node, key, value, index);
        }
    }

    int32_t inner_insert_nonfull(depth_type current_depth, default_node_type *node, uint32_t key, uint32_t value) {
        logged_task lt{ "inner-nonfull" };

        assert(node->type == node_type::Inner);
        assert(node->number_keys < InnerSize);
        assert(current_depth != 0);

        auto left = sector();

        phydebugf("%s entered (%d)", name(), dirty());

        insertion_t insertion;
        auto index = Keys::inner_position_for(key, *node);

        auto child_ptr = node->d.children[index];
        persisted_node_t followed;
        auto err = follow_node_ptr(child_ptr, followed);
        if (err < 0) {
            return err;
        }

        if (current_depth - 1 == 0) {
            // Children are leaf.
            err = leaf_node_insert(followed.node, followed.ptr, key, value, insertion);
            if (err < 0) {
                return err;
            }
        } else {
            // Children are inner.
            err = inner_node_insert(current_depth - 1, followed.node, followed.ptr, key, value, insertion);
            if (err < 0) {
                return err;
            }
        }

        if (left != sector()) {
            phydebugf("reloading previous");

            if (dirty()) {
                auto err = flush();
                if (err < 0) {
                    return err;
                }
            }

            assert(!dirty());

            sector(left);

            auto err =
                db().unsafe_all([&](uint8_t *buffer, size_t size) { return sectors_->read(sector(), buffer, size); });
            if (err < 0) {
                return err;
            }
        }

        assert(node->number_keys < InnerSize);

        if (insertion.split) {
            if (index == node->number_keys) {
                // Insertion at the rightmost key
                node->keys[index] = insertion.key;
                node->d.children[index] = insertion.left;
                node->d.children[index + 1] = insertion.right;
                node->number_keys++;
            } else {
                // Insertion not at the rightmost key
                node->d.children[node->number_keys + 1] = node->d.children[node->number_keys];
                for (auto i = node->number_keys; i != index; --i) {
                    node->d.children[i] = node->d.children[i - 1];
                    node->keys[i] = node->keys[i - 1];
                }
                node->d.children[index] = insertion.left;
                node->d.children[index + 1] = insertion.right;
                node->keys[index] = insertion.key;
                node->number_keys++;
            }

            phydebugf("recording split, dirty");

            dirty(true);
        } else {
            phydebugf("no changes");
        }

        return 0;
    }

    int32_t inner_node_insert(depth_type current_depth, default_node_type *node, node_ptr_t node_ptr, uint32_t key,
                              uint32_t value, insertion_t &insertion) {
        logged_task lt{ "inner-node" };

        assert(node->type == node_type::Inner);
        assert(current_depth != 0);

        // Early split if node is full.
        // This is not the canonical algorithm for B+ trees,
        // but it is simpler and does not break the definition.
        if (node->number_keys == InnerSize) {
            node_ptr_t ignored_ptr;
            auto err = allocate_node(ignored_ptr, [&](default_node_type *new_sibling, node_ptr_t &sibling_ptr) {
                auto treshold = (InnerSize + 1) / 2;

                new_sibling->type = node_type::Inner;
                new_sibling->number_keys = node->number_keys - treshold;
                for (auto i = 0; i < new_sibling->number_keys; ++i) {
                    new_sibling->keys[i] = node->keys[treshold + i];
                    new_sibling->d.children[i] = node->d.children[treshold + i];
                }

                new_sibling->d.children[new_sibling->number_keys] = node->d.children[node->number_keys];

                node->number_keys = treshold - 1;
                dirty(true);

                // Set up the return variable
                insertion.split = true;
                insertion.key = node->keys[treshold - 1];
                insertion.left = node_ptr;
                insertion.right = sibling_ptr;

                // Now insert in the appropriate sibling
                if (key < insertion.key) {
                    auto err = inner_insert_nonfull(current_depth, node, key, value);
                    if (err < 0) {
                        return err;
                    }
                } else {
                    auto err = inner_insert_nonfull(current_depth, new_sibling, key, value);
                    if (err < 0) {
                        return err;
                    }
                }

                return 0;
            });
            if (err < 0) {
                return err;
            }
        } else {
            auto err = inner_insert_nonfull(current_depth, node, key, value);
            if (err < 0) {
                return err;
            }
        }

        return 0;
    }

    int32_t flush() {
        if (dirty()) {
            auto err = sectors_->write(sector_, db().read_view().ptr(), db().read_view().size());
            if (err < 0) {
                return err;
            }

            dirty(false);
        } else {
            phydebugf("flush (noop)");
        }

        return 0;
    }

    template<typename TFill>
    int32_t allocate_node(node_ptr_t &ptr, TFill fill_fn) {
        db().seek_end();

        if (db().template room_for<default_node_type>()) {
            phydebugf("%s appending node %zu/%zu", name(), db().position(), db().size());

            auto placed = db().template reserve<default_node_type>();

            ptr = node_ptr_t{ sector_, placed.position };

            auto err = fill_fn(placed.record, ptr);
            if (err < 0) {
                return err;
            }

            dirty(true);

            return 0;
        }

        auto allocated = allocator_->allocate();
        assert(allocated != sector_); // Don't ask.

        phydebugf("%s grow! %zu/%zu alloc=%d", name(), db().position(), db().size(), allocated);

        delimited_buffer buffer{ buffer_allocator_.allocate(sector_size()) };
        auto placed = buffer.template reserve<default_node_type>();

        ptr = node_ptr_t{ allocated, placed.position };

        auto err = fill_fn(placed.record, ptr);
        if (err < 0) {
            return err;
        }

        err = sectors_->write(allocated, buffer.read_view().ptr(), buffer.read_view().size());
        if (err < 0) {
            return err;
        }

        return 0;
    }

    int32_t follow_node_ptr(node_ptr_t &ptr, persisted_node_t &followed) {
        if (ptr.sector != sector_) {
            phydebugf("follow %d:%d (load-sector)", ptr.sector, ptr.position);

            auto err = flush();
            if (err < 0) {
                return err;
            }

            assert(!dirty());

            err = db().unsafe_all([&](uint8_t *buffer, size_t size) { return sectors_->read(ptr.sector, buffer, size); });
            if (err < 0) {
                return err;
            }

            sector(ptr.sector);
        } else {
            phydebugf("follow %d:%d (same-sector)", ptr.sector, ptr.position);
        }

        followed = find_node_in_sector(db(), ptr);
        if (followed.node == nullptr) {
            phyerrorf("follow: unable to find node in sector");
            return -1;
        }

        return 0;
    }

    int32_t back_to_root() {
        if (sector_ != root_) {
            phydebugf("%s back-to-root %d -> %d", name(), sector_, root_);
            sector(root_);

            assert(!dirty());
            auto err =
                db().unsafe_all([&](uint8_t *buffer, size_t size) { return sectors_->read(sector_, buffer, size); });
            if (err < 0) {
                return err;
            }
        } else {
            phydebugf("%s back-to-root %d (NOOP)", name(), sector_);
        }

        db().rewind();

        return 0;
    }

    int32_t log(default_node_type *node) {
        logged_task it{ name() };

        phyinfof("node %d:%d depth=%d nkeys=%d", sector(), db().position(), node->depth, node->number_keys);

        if (node->type == node_type::Inner) {
            for (auto i = 0u; i <= node->number_keys; ++i) {
                auto child = node->d.children[i];

                phyinfof("inner #%d key=%d -> %d:%d", i, node->keys[i], child.sector, child.position);

                auto left = sector();

                persisted_node_t followed;
                auto err = follow_node_ptr(child, followed);
                if (err < 0) {
                    return err;
                }

                err = log(followed.node);
                if (err < 0) {
                    return err;
                }

                if (left != sector()) {
                    phyinfof("reloading");
                    sector(left);

                    auto err = db().unsafe_all(
                        [&](uint8_t *buffer, size_t size) { return sectors_->read(sector(), buffer, size); });
                    if (err < 0) {
                        return err;
                    }
                }
            }
        } else {
            for (auto i = 0u; i < node->number_keys; ++i) {
                phyinfof("leaf #%d %d = %d", i, node->keys[i], node->d.values[i]);
            }
        }

        return 0;
    }

public:
    int32_t create() {
        logged_task lt{ "tree-create" };

        phydebugf("%s creating", name());

        assert(sector_ == InvalidSector);

        sector(root_);

        assert(db().empty());

        phydebugf("creating new node");
        db().template emplace<default_node_type>(node_type::Leaf);
        dirty(true);

        auto err = flush();
        if (err < 0) {
            return err;
        }

        return 0;
    }

    int32_t add(uint32_t key, uint32_t value) {
        logged_task lt{ "tree-add" };

        phydebugf("%s adding %d = %d", name(), key, value);

        auto err = back_to_root();
        if (err < 0) {
            return err;
        }

        auto pnode = find_sector_root(sector(), db());
        assert(pnode.node != nullptr);

        auto node = pnode.node;
        auto node_ptr = pnode.ptr;

        insertion_t insertion;

        if (node->depth == 0) {
            auto err = leaf_node_insert(node, node_ptr, key, value, insertion);
            if (err < 0) {
                return err;
            }
        } else {
            auto err = inner_node_insert(node->depth, node, node_ptr, key, value, insertion);
            if (err < 0) {
                return err;
            }
        }

        if (insertion.split) {
            // The old root was separated in two parts.
            // We have to create a new root pointing to them
            node_ptr_t ptr;
            auto err = allocate_node(ptr, [&](default_node_type *new_node, node_ptr_t &ignored_ptr) {
                new_node->type = node_type::Inner;
                new_node->depth = node->depth + 1;
                new_node->number_keys = 1;
                new_node->keys[0] = insertion.key;
                new_node->d.children[0] = insertion.left;
                new_node->d.children[1] = insertion.right;
                return 0;
            });
            if (err < 0) {
                return err;
            }

            root_ = ptr.sector;

            phydebugf("%s new top", name());
        }

        err = flush();
        if (err < 0) {
            return err;
        }

        phydebugf("%s done adding %d = %d", name(), key, value);

        return 0;
    }

    int32_t find(KEY key, VALUE *value = 0) {
        logged_task lt{ "tree-find" };

        phydebugf("finding %d", key);

        assert(!dirty());

        auto err = back_to_root();
        if (err < 0) {
            return err;
        }

        auto pnode = find_sector_root(sector(), db());
        auto node = pnode.node;
        assert(node != nullptr);

        auto starting_depth = node->depth;
        auto d = starting_depth;
        while (d-- != 0 && node->type == node_type::Inner) {
            auto index = Keys::inner_position_for(key, *node);
            assert(index < node->number_keys + 1);

            auto child_ptr = node->d.children[index];
            persisted_node_t followed;
            auto err = follow_node_ptr(child_ptr, followed);
            if (err < 0) {
                return err;
            }

            node = followed.node;
        }

        assert(node->type == node_type::Leaf);
        auto index = Keys::leaf_position_for(key, *node);
        assert(index <= node->number_keys);
        if (index < node->number_keys && node->keys[index] == key) {
            if (value != nullptr) {
                *value = node->d.values[index];
            }
            return 1;
        }

        return 0;
    }

    bool find_last_less_then(const KEY &key, VALUE *value = 0, KEY *out_key = 0) {
        logged_task lt{ "tree-find-less" };

        phydebugf("finding %d", key);

        assert(!dirty());

        auto err = back_to_root();
        if (err < 0) {
            return err;
        }

        auto pnode = find_sector_root(sector(), db());
        auto node = pnode.node;
        assert(node != nullptr);


        auto starting_depth = node->depth;
        auto d = starting_depth;
        while (d-- != 0 && node->type == node_type::Inner) {
            auto index = Keys::inner_position_for(key, *node);
            assert(index < node->number_keys + 1);

            if (index > 0 && key == node->keys[index - 1]) {
                index -= 1;
            }
            assert(index == 0 || node->keys[index - 1] < key);

            auto child_ptr = node->d.children[index];
            persisted_node_t followed;
            auto err = follow_node_ptr(child_ptr, followed);
            if (err < 0) {
                return err;
            }

            node = followed.node;
        }

        assert(node->type == node_type::Leaf);
        auto index = Keys::leaf_position_for(key, *node);

        if (index <= node->number_keys) {
            index -= 1;
            if (index < node->number_keys && key == node->keys[index]) {
                index -= 1;
            }

            if (index < node->number_keys) {
                assert(node->keys[index] < key);

                if (value != nullptr) {
                    *value = node->d.values[index];
                }
                if (out_key != nullptr) {
                    *out_key = node->keys[index];
                }

                return 1;
            }
        }

        return 0;
    }

    int32_t log() {
        logged_task lt{ "tree-log" };

        assert(!dirty());

        auto err = back_to_root();
        if (err < 0) {
            return err;
        }

        auto pnode = find_sector_root(sector(), db());
        auto node = pnode.node;
        assert(node != nullptr);

        return log(node);
    }

};

} // namespace phylum
