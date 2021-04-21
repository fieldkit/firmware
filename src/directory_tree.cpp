#include "directory_tree.h"
#include "data_chain.h"
#include "tree_sector.h"

namespace phylum {

int32_t directory_tree::mount() {
    if (!tree_.exists()) {
        return -1;
    }

    return 0;
}

int32_t directory_tree::format() {
    return tree_.create();
}

int32_t directory_tree::touch(const char *name) {
    auto id = make_file_id(name);

    node_ = {};
    node_.u.file = dirtree_file_t(name);

    file_ = {};
    file_.id = id;

    auto err = tree_.add(id, node_);
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t directory_tree::unlink(const char *name) {
    auto id = make_file_id(name);

    node_ = {};
    node_.u.file = dirtree_file_t(name, (uint16_t)FsDirTreeFlags::Deleted);

    file_ = {};

    auto err = tree_.add(id, node_);
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t directory_tree::find(const char *name, open_file_config file_cfg) {
    auto id = make_file_id(name);

    file_ = found_file{};
    file_.cfg = file_cfg;
    file_.id = id;

    // Zero attribute values before we scan.
    for (auto i = 0u; i < file_cfg.nattrs; ++i) {
        auto &attr = file_cfg.attributes[i];
        bzero(attr.ptr, attr.size);
    }

    auto err = tree_.find(id, &node_);
    if (err < 0) {
        file_ = found_file{};
        return err;
    }

    if (err == 0) {
        file_ = found_file{};
        return 0;
    }

    auto mask = (uint16_t)FsDirTreeFlags::Deleted;
    if ((node_.u.e.flags & mask) == mask) {
        file_ = found_file{};
        return 0;
    }

    assert(node_.u.e.type == entry_type::FsFileEntry);

    if (!node_.u.file.chain.valid()) {
        if (((int32_t)file_cfg.flags & (int32_t)open_file_flags::Truncate) == 0) {
            file_.directory_size = node_.u.file.directory_size;
            file_.directory_capacity = DataCapacity - node_.u.file.directory_size;
        }
        else {
            file_.directory_size = 0;
            file_.directory_capacity = DataCapacity;
        }
    }
    else {
        file_.chain = node_.u.file.chain;
        file_.position_index = node_.u.file.position_index;
        file_.record_index = node_.u.file.record_index;
    }

    // If we're being asked to load attributes.
    if (file_cfg.nattrs > 0) {
        if (node_.u.file.attributes.valid()) {
            attr_tree_type tree{ phyctx{ *buffers_, *sectors_, *allocator_ }, node_.u.file.attributes, "attrs" };
            for (auto i = 0u; i < file_cfg.nattrs; ++i) {
                auto &attr = file_cfg.attributes[i];
                attr_node_type attr_node;

                auto err = tree.find(attr.type, &attr_node);
                if (err < 0) {
                    return err;
                }

                assert(attr.size <= AttributeCapacity);
                memcpy(attr.ptr, attr_node.data, attr.size);
            }
        }
    }

    return 1;
}

found_file directory_tree::open() {
    assert(file_.id != UINT32_MAX);
    return file_;
}

int32_t directory_tree::file_data(file_id_t id, file_size_t position, uint8_t const *buffer, size_t size) {
    assert(file_.id == id);

    if (position + size > DataCapacity) {
        return -1;
    }

    memcpy(node_.data + position, buffer, size);

    node_.u.file.directory_size = position + size;

    auto err = flush();
    if (err < 0) {
        return err;
    }

    return size;
}

int32_t directory_tree::file_chain(file_id_t id, head_tail_t chain) {
    assert(file_.id == id);

    assert(!node_.u.file.chain.valid());

    node_.u.file.directory_size = 0;
    node_.u.file.chain = chain;

    auto err = flush();
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t directory_tree::file_attributes(file_id_t id, open_file_attribute *attributes, size_t nattrs) {
    assert(file_.id == id);

    auto attribute_size = 0u;
    for (auto i = 0u; i < nattrs; ++i) {
        assert(attributes[i].size <= AttributeCapacity);
        attribute_size += attributes[i].size;
    }

    phydebugf("saving attributes total-size=%zu", attribute_size);

    // TODO Store attributes in inline data when we can.
    auto attributes_tree = node_.u.file.attributes;
    auto create = false;
    if (!node_.u.file.attributes.valid()) {
        auto allocated = allocator_->allocate();
        attributes_tree = tree_ptr_t{ allocated, allocated };
        create = true;
    }

    attr_tree_type tree{ phyctx{ *buffers_, *sectors_, *allocator_ }, attributes_tree, "attrs" };

    if (create) {
        auto err = tree.create();
        if (err < 0) {
            return err;
        }
    }

    for (auto i = 0u; i < nattrs; ++i) {
        auto &attr = attributes[i];
        auto err = tree.add(attr.type, attr_node_type{ attr.ptr, attr.size });
        if (err < 0) {
            return err;
        }
    }

    if (node_.u.file.attributes != attributes_tree) {
        node_.u.file.attributes = attributes_tree;

        auto err = flush();
        if (err < 0) {
            return err;
        }
    }

    return 0;
}

int32_t directory_tree::file_trees(file_id_t id, tree_ptr_t position_index, tree_ptr_t record_index) {
    assert(file_.id == id);

    node_.u.file.position_index = position_index;
    node_.u.file.record_index = record_index;

    auto err = flush();
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t directory_tree::read(file_id_t id, std::function<int32_t(read_buffer)> fn) {
    assert(file_.id == id);

    if (node_.u.file.directory_size == 0) {
        return 0;
    }

    return fn(read_buffer{ node_.data, node_.u.file.directory_size });
}

int32_t directory_tree::flush() {
    assert(file_.id != UINT32_MAX);

    auto err = tree_.add(file_.id, node_);
    if (err < 0) {
        return err;
    }

    return 0;
}

} // namespace phylum
