#pragma once

#include "sector_map.h"
#include "sector_allocator.h"
#include "delimited_buffer.h"
#include "working_buffers.h"
#include "paging_delimited_buffer.h"
#include "phyctx.h"

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

struct tree_value_ptr_t {
    node_ptr_t node;
    index_type index;
};

template <typename KEY, typename VALUE, size_t Size>
class tree_sector {
public:
    static constexpr size_t NodeSize = Size;
    using key_type = KEY;
    using value_type = VALUE;
    using default_node_type = tree_node_t<KEY, VALUE, Size>;

private:
    static constexpr size_t ScopeNameLength = 32;

    struct insertion_t {
        bool split{ false };
        KEY key{ 0 };
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
    using buffer_type = paging_delimited_buffer;
    working_buffers *buffers_{ nullptr };
    sector_map *sectors_{ nullptr };
    sector_allocator *allocator_{ nullptr };
    dhara_sector_t root_{ InvalidSector };
    dhara_sector_t tail_{ InvalidSector };
    const char *prefix_{ "tree-sector" };
    char name_[ScopeNameLength];

public:
    tree_sector(phyctx pc, tree_ptr_t tree, const char *prefix = "tree")
        : buffers_(&pc.buffers_), sectors_(&pc.sectors_), allocator_(&pc.allocator_), root_(tree.root), tail_(tree.tail), prefix_(prefix) {
        name("%s[%d]", prefix_, root_);
    }

    tree_sector(tree_sector &other, tree_ptr_t tree, const char *prefix)
        : buffers_(other.buffers_), sectors_(other.sectors_), allocator_(other.allocator_),
          root_(tree.root), tail_(tree.tail), prefix_(prefix) {
        name("%s[%d]", prefix_, root_);
    }

    virtual ~tree_sector() {
    }

public:
    const char *name() const {
        return name_;
    }

    tree_ptr_t to_tree_ptr() const {
        return tree_ptr_t{ root_, tail_ };
    }

protected:
    sector_allocator &allocator() {
        return *allocator_;
    }

private:
    static persisted_node_t find_root_in_sector(dhara_sector_t sector, delimited_buffer &db) {
        persisted_node_t selected;
        for (auto iter = db.begin(); iter != db.end(); ++iter) {
            auto rp = *iter;
            if (rp.as<entry_t>()->type == entry_type::TreeNode) {
                auto node = db.as_mutable<default_node_type>(rp);
                if (selected.node == nullptr || selected.node->depth < node->depth) {
                    selected = persisted_node_t{ node, node_ptr_t{ sector, (sector_offset_t)rp.position() } };
                }
            }
        }
        phyverbosef("found root=%d:%d", selected.ptr.sector, selected.ptr.position);
        return selected;
    }

    static persisted_node_t find_node_in_sector(delimited_buffer &db, node_ptr_t ptr) {
        persisted_node_t selected;
        for (auto iter = db.begin(); iter != db.end(); ++iter) {
            auto rp = *iter;
            if (rp.position() == ptr.position) {
                auto node = db.as_mutable<default_node_type>(rp);
                return persisted_node_t{ node, ptr };
            }
        }
        return persisted_node_t{};
    }

private:
    void name(const char *f, ...) {
        va_list args;
        va_start(args, f);
        phy_vsnprintf(name_, sizeof(name_), f, args);
        va_end(args);
    }

    template<typename TAccess>
    int32_t dereference_root(TAccess fn) {
        buffer_type buffer{ *buffers_, *sectors_ };

        phyverbosef("dereference-root: %d", root_);

        auto lock = buffer.writing(root_);

        auto node = find_root_in_sector(root_, buffer);

        auto err = fn(lock, node.node, node.ptr);
        if (err < 0) {
            return err;
        }

        if (lock.is_dirty()) {
            return lock.flush(lock.sector());
        }

        return 0;
    }

    template<typename TAccess>
    int32_t dereference(bool read_only, node_ptr_t node_ptr, TAccess fn) {
        assert(node_ptr.sector != InvalidSector);

        buffer_type buffer{ *buffers_, *sectors_ };

        phyverbosef("dereference-node: %d:%d", node_ptr.sector, node_ptr.position);

        auto lock = read_only ? buffer.reading(node_ptr.sector) : buffer.writing(node_ptr.sector);

        auto node = find_node_in_sector(buffer, node_ptr);

        auto err = fn(lock, node.node);
        if (err < 0) {
            return err;
        }

        if (lock.is_dirty()) {
            return lock.flush(lock.sector());
        }

        return 0;
    }

    int32_t flush(page_lock &lock) {
        auto err = lock.flush(lock.sector());
        if (err < 0) {
            return err;
        }

        return 0;
    }

    template<typename TFill>
    int32_t allocate_node(page_lock &lock, node_ptr_t &ptr, TFill fill_fn) {
        {
            auto &db = lock.db();

            db.seek_end();

            if (db.template room_for<default_node_type>()) {
                phydebugf("%s appending node %d:%d (%d)", name(), lock.sector(), db.position(), sizeof(default_node_type));

                auto placed = db.template reserve<default_node_type>();

                ptr = node_ptr_t{ lock.sector(), placed.position };

                placed.record->dbg.sector = lock.sector();

                lock.dirty();

                phyverbosef("allocate-node filling");

                auto err = fill_fn(lock, placed.record, ptr);
                if (err < 0) {
                    return err;
                }

                phyverbosef("allocate-node done filling");

                return 0;
            }
            else {
                phydebugf("%s page full %zu/%zu (%zu)", name(), db.position(), db.size(), db.available());
            }
        }

        buffer_type buffer{ *buffers_, *sectors_ };

        auto allocated = allocator_->allocate();

        auto child_lock = buffer.writing(allocated);

        auto &db = child_lock.db();

        phydebugf("%s grow! allocated=%d", name(), allocated);

        db.rewind();

        db.template emplace<sector_chain_header_t>(entry_type::TreeSector, InvalidSector, tail_);

        tail_ = allocated;

        auto placed = db.template reserve<default_node_type>();

        phydebugf("creating new node %d:%d node-size=%d sector-size=%d",
                  allocated, db.position(), sizeof(default_node_type), db.size());

        ptr = node_ptr_t{ allocated, placed.position };

        placed.record->dbg.sector = child_lock.sector();

        phyverbosef("allocate-node filling");

        auto err = fill_fn(child_lock, placed.record, ptr);
        if (err < 0) {
            return err;
        }

        phyverbosef("allocate-node done filling");

        err = child_lock.flush(allocated);
        if (err < 0) {
            return err;
        }

        return 0;
    }

    int32_t follow_node_ptr(page_lock &lock, node_ptr_t &ptr, persisted_node_t &followed) {
        if (ptr.sector != lock.sector()) {
            phyinfof("follow %d -> %d:%d (load-sector)", lock.sector(), ptr.sector, ptr.position);

            assert(!lock.is_dirty());

            auto err = lock.replace(ptr.sector);
            if (err < 0) {
                return err;
            }

            phydebugf("follow %d:%d (done) page-lock-sector=%d", ptr.sector, ptr.position, lock.sector());
        } else {
            phyinfof("follow %d -> %d:%d (same-sector)", lock.sector(), ptr.sector, ptr.position);
        }

        followed = find_node_in_sector(lock.db(), ptr);
        if (followed.node == nullptr) {
            phyerrorf("follow: unable to find node in sector");
            return -1;
        }

        return 0;
    }

    int32_t back_to_root(page_lock &lock) {
        phyverbosef("%s back-to-root %d -> %d", name(), lock.sector(), root_);

        assert(!lock.is_dirty());

        auto err = lock.replace(root_);
        if (err < 0) {
            return err;
        }

        return 0;
    }

    int32_t split_child(page_lock &lock, index_type index, node_ptr_t node_ptr, default_node_type *node, node_ptr_t child_ptr, default_node_type *child) {
        // Node has children which child should be one of and so has to be an inner node.
        assert(node->type == node_type::Inner);

        node_ptr_t allocated_ptr;
        auto err = allocate_node(lock, allocated_ptr, [this, index, &node_ptr, node, &child_ptr, child](page_lock &new_lock, default_node_type *new_node, node_ptr_t new_node_ptr) -> int32_t {
            index_type threshold = (Size + 1) / 2;

            assert(child->number_keys >= threshold);

            phydebugf("splitting child-nkeys=%d parent=%d:%d child=%d:%d new-child=%d:%d type=%s threshold=%d",
                      child->number_keys,
                      node_ptr.sector, node_ptr.position,
                      child_ptr.sector, child_ptr.position,
                      new_node_ptr.sector, new_node_ptr.position,
                      child->type == node_type::Inner ? "inner" : "leaf", threshold);

            new_node->type = child->type;
            new_node->depth = node->depth;
            new_node->number_keys = child->number_keys - threshold;

            new_lock.dirty();

            for (auto j = 0; j < new_node->number_keys; ++j) {
                new_node->keys[j] = child->keys[j + threshold];
            }

            if (child->type == node_type::Inner) {
                for (auto j = 0; j <= new_node->number_keys; ++j) {
                    new_node->d.children[j] = child->d.children[j + threshold];
                }
            }
            else {
                assert(child->type == node_type::Leaf);
                for (auto j = 0; j < new_node->number_keys; ++j) {
                    phyverbosef("copying %d:%d #%d -> %d:%d #%d",
                                child_ptr.sector, child_ptr.position, j + threshold,
                                new_node_ptr.sector, new_node_ptr.position, j);
                    new_node->d.values[j] = child->d.values[j + threshold];
                }
            }

            child->number_keys = threshold;

            for (auto j = node->number_keys; j >= index + 1; j--) {
                node->d.children[j + 1] = node->d.children[j];
            }

            phyverbosef("linking child %d:%d #%d = %d:%d", node_ptr.sector, node_ptr.position, index + 1, new_node_ptr.sector, new_node_ptr.position);

            node->d.children[index + 1] = new_node_ptr;

            for (auto j = node->number_keys - 1; j >= index; j--) {
                node->keys[j + 1] = node->keys[j];
            }

            node->keys[index] = new_node->keys[0];

            node->number_keys++;

            phyverbosef("moved key %d nkeys=%d", new_node->keys[0], node->number_keys);

            return 0;
        });
        if (err < 0) {
            return err;
        }
        return 0;
    }

    int32_t insert_non_full(node_ptr_t node_ptr, KEY &key, VALUE *value, tree_value_ptr_t *found_ptr) {
        node_ptr_t insertion_ptr;

        auto err = dereference(false, node_ptr, [this, &insertion_ptr, node_ptr, &key, &value, found_ptr](page_lock &lock, default_node_type *node) -> int32_t {
            index_type index = node->number_keys - 1;

            assert(node->number_keys < (index_type)Size);

            if (node->type == node_type::Leaf) {
                // Check for an overwrite before we shift. Is there a
                // faster way?
                auto overwrite = false;
                for (auto i = 0; i < node->number_keys; ++i) {
                    if (node->keys[i] == key) {
                        phydebugf("replace leaf=%d:%d index=%d key=%d nkeys=%d", node_ptr.sector, node_ptr.position, i, key, node->number_keys);
                        if (value != nullptr) {
                            node->d.values[i] = *value;
                        }
                        if (found_ptr != nullptr) {
                            found_ptr->node = node_ptr;
                            found_ptr->index = i;
                        }
                        overwrite = true;
                        break;
                    }
                }

                if (!overwrite) {
                    while (index >= 0 && node->keys[index] > key) {
                        node->keys[index + 1] = node->keys[index];
                        node->d.values[index + 1] = node->d.values[index];
                        index--;
                    }

                    phydebugf("value leaf=%d:%d index=%d key=%d nkeys=%d", node_ptr.sector, node_ptr.position, index + 1, key, node->number_keys);
                    node->keys[index + 1] = key;
                    if (value != nullptr) {
                        node->d.values[index + 1] = *value;
                    }
                    if (found_ptr != nullptr) {
                        found_ptr->node = node_ptr;
                        found_ptr->index = index + 1;
                    }
                    node->number_keys++;
                }

                lock.dirty();
            }
            else {
                while (index >= 0 && node->keys[index] > key) {
                    index--;
                }

                auto child_ptr = node->d.children[index + 1];
                auto err = dereference(false, child_ptr, [this, &lock, &index, &key, child_ptr, node_ptr, node](page_lock &child_lock, default_node_type *child) -> int32_t {
                    if (child->number_keys == Size) {
                        phydebugf("splitting child %d:%d", child_ptr.sector, child_ptr.position);

                        auto err = split_child(lock, index + 1, node_ptr, node, child_ptr, child);
                        if (err < 0) {
                            return err;
                        }

                        lock.dirty();

                        child_lock.dirty();

                        if (node->keys[index + 1] < key) {
                            index++;
                        }
                    }
                    return 0;
                });
                if (err < 0) {
                    return err;
                }

                // Index can change above, so we dereference again and
                // then yield this to the outer scope so we can
                // release this one.
                insertion_ptr = node->d.children[index + 1];

                return 0;
            }

            return 0;
        });
        if (err < 0) {
            return err;
        }

        if (insertion_ptr.valid()) {
            err = insert_non_full(insertion_ptr, key, value, found_ptr);
            if (err < 0) {
                return err;
            }
        }

        return 0;
    }

    int32_t log_node(node_ptr_t node_ptr, default_node_type *node) {
        name("%s[%d]", prefix_, node_ptr.sector);

        // logged_task it{ name() };

        if (node->type == node_type::Inner) {
            phyinfof("inner nkeys=%d", node->number_keys);
            for (auto i = 0; i <= node->number_keys; ++i) {
                auto child = node->d.children[i];
                phyinfof("inner %d:%d #%d key=%d -> %d:%d", node_ptr.sector, node_ptr.position, i, node->keys[i], child.sector, child.position);
            }
        }
        else {
            phyinfof("leaf nkeys=%d", node->number_keys);

            for (auto i = 0; i < node->number_keys; ++i) {
                phyinfof("leaf %d:%d #%d key=%d = %d", node_ptr.sector, node_ptr.position, i, node->keys[i], node->d.values[i]);
            }
        }

        return 0;
    }

    int32_t log(node_ptr_t node_ptr, bool graph) {
        name("%s[%d]", prefix_, node_ptr.sector);

        // logged_task it{ name() };

        for (auto i = 0; i < (index_type)Size; ++i) {
            node_ptr_t follow_ptr;

            // We recurse outside of the dereference lambda so that we
            // are only ever consuming the minimum number of pages.
            auto err = dereference(true, node_ptr, [this, &i, &node_ptr, &follow_ptr](page_lock &/*lock*/, default_node_type *node) -> int32_t {
                if (node->type == node_type::Inner) {
                    if (i == 0) {
                        phyinfof("inner nkeys=%d", node->number_keys);
                    }

                    if (i <= node->number_keys) {
                        auto child = node->d.children[i];
                        phyinfof("inner %d:%d #%d key=%d -> %d:%d", node_ptr.sector, node_ptr.position, i, node->keys[i], child.sector, child.position);
                        follow_ptr = child;
                    }
                    else {
                        i = Size;
                    }
                }
                else {
                    if (i == 0) {
                        phyinfof("leaf nkeys=%d", node->number_keys);

                        for (auto j = 0; j < node->number_keys; ++j) {
                            phyinfof("leaf %d:%d #%d key=%d = (%d bytes)", node_ptr.sector, node_ptr.position, j, node->keys[j], sizeof(VALUE));
                        }
                    }

                    i = Size;
                }
                return 0;
            });
            if (err < 0) {
                return err;
            }

            if (follow_ptr.valid()) {
                auto err = log(follow_ptr, graph);
                if (err < 0) {
                    return err;
                }
            }
        }

        return 0;
    }

public:
    int32_t exists() {
        logged_task lt{ "tree-exists" };

        dhara_page_t page = 0;
        auto err = sectors_->find(root_, &page);
        if (err < 0) {
            return 0;
        }

        phydebugf("exists!");

        return 1;
    }

    int32_t create() {
        if (root_ == InvalidSector) {
            root_ = allocator_->allocate();
            tail_ = root_;
            phydebugf("tree-create allocating sector=%d", root_);
        }

        logged_task lt{ name(), "tree-create" };

        phydebugf("creating");

        buffer_type db{ *buffers_, *sectors_ };

        auto lock = db.overwrite(root_);

        assert(db.empty());

        db.template emplace<sector_chain_header_t>(entry_type::TreeSector);

        phydebugf("creating new tree position=%d node-size=%d sector-size=%d", db.position(), sizeof(default_node_type), db.size());

        auto placed = db.template reserve<default_node_type>();

        placed.record->type = node_type::Leaf;
        placed.record->dbg.sector = lock.sector();

        lock.dirty();

        auto err = flush(lock);
        if (err < 0) {
            return err;
        }

        return 0;
    }

    template<typename ModifyFunction>
    int32_t modify_in_place(tree_value_ptr_t value_ptr, ModifyFunction fn) {
        auto err = dereference(false, value_ptr.node, [&](page_lock &lock, default_node_type *node) -> int32_t {
            auto value = &node->d.values[value_ptr.index];

            auto err = fn(value);
            if (err < 0) {
                return err;
            }

            if (err > 0) {
                lock.dirty();
            }

            return 0;
        });
        if (err < 0) {
            return err;
        }
        return 0;
    }

    int32_t add(KEY key, VALUE value) {
        return add(key, &value, nullptr);
    }

    int32_t add(KEY key, VALUE *value, tree_value_ptr_t *found_ptr) {
        logged_task lt{ name(), "tree-add" };

        assert(root_ != InvalidSector);

        phydebugf("adding node");

        node_ptr_t insertion_ptr;

        auto err = dereference_root([this, &insertion_ptr, &key, &value, found_ptr](page_lock &lock, default_node_type *node, node_ptr_t node_ptr) -> int32_t {
            phydebugf("adding node depth=%d", node->depth);

            if (node->number_keys == 0) {
                assert(node->type == node_type::Leaf);

                node->keys[0] = key;
                if (value != nullptr) {
                    node->d.values[0] = *value;
                }
                if (found_ptr != nullptr) {
                    found_ptr->node = node_ptr;
                    found_ptr->index = 0;
                }
                node->number_keys++;

                phydebugf("value leaf=%d:%d index=%d key=%d (root)", node_ptr.sector, node_ptr.position, 0, key);

                lock.dirty();
            }
            else if (node->number_keys == Size) {
                phydebugf("root full, growing tree");

                node_ptr_t allocated_ptr;
                auto err = allocate_node(lock, allocated_ptr,
                                         [this, &lock, &insertion_ptr, &key, &value, node, &node_ptr]
                                         (page_lock &new_lock, default_node_type *new_node, node_ptr_t new_node_ptr) -> int32_t {

                    // This is unusual and the reason we do this is so
                    // that the root node is always in the same
                    // place. This has several benefits, the most
                    // notable of which is that we don't need to
                    // update references to this tree when we add
                    // values and we keep the head of the "chain"
                    // formed by the sectors in the tree.
                    *new_node = *node;

                    new_lock.dirty();

                    new (node) default_node_type{ };
                    node->type = node_type::Inner;
                    node->depth = new_node->depth + 1;
                    node->d.children[0] = new_node_ptr;

                    auto err = split_child(new_lock, 0, node_ptr, node, new_node_ptr, new_node);
                    if (err < 0) {
                        return err;
                    }

                    lock.dirty();

                    auto index = 0;
                    if (node->keys[0] < key) {
                        index++;
                    }

                    insertion_ptr = node->d.children[index];

                    return 0;
                });
                if (err < 0) {
                    return err;
                }

                phydebugf("root full, root=%d:%d old-root=%d:%d", node_ptr.sector, node_ptr.position,
                          allocated_ptr.sector, allocated_ptr.position);
            }
            else {
                insertion_ptr = node_ptr;
            }

            return 0;
        });
        if (err < 0) {
            return err;
        }

        if (insertion_ptr.valid()) {
            auto err = insert_non_full(insertion_ptr, key, value, found_ptr);
            if (err < 0) {
                return err;
            }
        }

        phydebugf("done adding");

        return 0;
    }

    int32_t find(KEY key, VALUE *value = nullptr, tree_value_ptr_t *found_ptr = nullptr) {
        logged_task lt{ name(), "tree-find" };

        phydebugf("finding %d", key);

        buffer_type db{ *buffers_, *sectors_ };

        auto lock = db.reading(root_);

        auto err = back_to_root(lock);
        if (err < 0) {
            return err;
        }

        auto pnode = find_root_in_sector(lock.sector(), db);
        auto node = pnode.node;
        auto node_ptr = pnode.ptr;

        assert(node != nullptr);

        auto starting_depth = node->depth;
        auto d = starting_depth;
        while (d-- != 0 && node->type == node_type::Inner) {
            auto index = Keys::inner_position_for(key, *node);
            assert(index < node->number_keys + 1);

            auto child_ptr = node->d.children[index];
            persisted_node_t followed;
            auto err = follow_node_ptr(lock, child_ptr, followed);
            if (err < 0) {
                return err;
            }

            node = followed.node;
            node_ptr = followed.ptr;
        }

        assert(node->type == node_type::Leaf);
        auto index = Keys::leaf_position_for(key, *node);
        assert(index <= node->number_keys);
        if (index < node->number_keys && node->keys[index] == key) {
            phydebugf("found! %d:%d #%d key=%d", node_ptr.sector, node_ptr.position, index, key);
            if (value != nullptr) {
                *value = node->d.values[index];
            }
            if (found_ptr != nullptr) {
                found_ptr->node = node_ptr;
                found_ptr->index = index;
            }
            return 1;
        }

        return 0;
    }

    bool find_last_less_then(const KEY &key, VALUE *value = 0, KEY *out_key = 0) {
        logged_task lt{ name(), "tree-find-less" };

        phydebugf("finding %d", key);

        buffer_type db{ *buffers_, *sectors_ };

        auto lock = db.reading(root_);

        auto err = back_to_root(lock);
        if (err < 0) {
            return err;
        }

        auto pnode = find_root_in_sector(lock.sector(), db);
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
            auto err = follow_node_ptr(lock, child_ptr, followed);
            if (err < 0) {
                return err;
            }

            node = followed.node;
        }

        assert(node->type == node_type::Leaf);
        auto index = Keys::leaf_position_for(key, *node);

        if (index <= node->number_keys) {
            if (index > 0) {
                index -= 1;
            }
            if (index < node->number_keys && key == node->keys[index]) {
                if (index > 0) {
                    index -= 1;
                }
            }

            if (index < node->number_keys) {
                assert(node->keys[index] <= key);

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

    int32_t log(bool graph = false) {
        logged_task lt{ name(), "tree-log" };

        buffer_type db{ *buffers_, *sectors_ };

        auto lock = db.reading(root_);

        auto err = back_to_root(lock);
        if (err < 0) {
            return err;
        }

        auto pnode = find_root_in_sector(lock.sector(), db);

        return log(pnode.ptr, graph);
    }

};

} // namespace phylum
