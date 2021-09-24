#include "directory_tree.h"
#include "data_chain.h"
#include "tree_sector.h"
#include "tree_attribute_storage.h"

namespace phylum {

int32_t directory_tree::mount() {
    logged_task lt{ "dir-tree-mount" };

    if (!tree_.exists()) {
        return -1;
    }

    return 0;
}

int32_t directory_tree::format() {
    logged_task lt{ "dir-tree-format" };

    return tree_.create();
}

int32_t directory_tree::touch(const char *name) {
    logged_task lt{ "dir-tree-touch" };

    phydebugf("touch '%s'", name);

    auto id = make_file_id(name);

    file_ = {};
    file_.id = id;

    auto err = tree_.add(id, nullptr, &file_node_ptr_);
    if (err < 0) {
        return err;
    }

    err = flush([&](dir_node_type *node) -> int32_t {
        *node = dir_node_type{};
        node->u.file = dirtree_file_t(name);
        return 1;
    });
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t directory_tree::unlink(const char *name) {
    logged_task lt{ "dir-tree-unlink" };

    phydebugf("unlink '%s'", name);

    auto id = make_file_id(name);

    file_ = {};

    auto err = tree_.find(id, nullptr, &file_node_ptr_);
    if (err < 0) {
        return err;
    }

    err = flush([&](dir_node_type *node) -> int32_t {
        *node = dir_node_type{};
        node->u.file = dirtree_file_t(name, (uint16_t)FsDirTreeFlags::Deleted);
        return 1;
    });
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t directory_tree::find(const char *name, open_file_config file_cfg) {
    logged_task lt{ "dir-tree-find" };

    phydebugf("finding '%s'", name);

    auto id = make_file_id(name);

    file_ = found_file{};
    file_.cfg = file_cfg;
    file_.id = id;

    // Zero attribute values before we scan.
    for (auto i = 0u; i < file_cfg.nattrs; ++i) {
        auto &attr = file_cfg.attributes[i];
        bzero(attr.ptr, attr.size);
    }

    auto err = tree_.find(id, nullptr, &file_node_ptr_);
    if (err < 0) {
        file_ = found_file{};
        return err;
    }
    if (err == 0) {
        phydebugf("no file");
        return 0;
    }

    auto deleted = false;

    err = flush([&](dir_node_type *node) -> int32_t {
        auto mask = (uint16_t)FsDirTreeFlags::Deleted;
        if ((node->u.e.flags & mask) == mask) {
            deleted = true;
            return 0;
        }

        assert(node->u.e.type == entry_type::FsFileEntry);

        if (!node->u.file.chain.valid()) {
            if (((int32_t)file_cfg.flags & (int32_t)open_file_flags::Truncate) == 0) {
                file_.directory_size = node->u.file.directory_size;
                file_.directory_capacity = DataCapacity - node->u.file.directory_size;
            }
            else {
                file_.directory_size = 0;
                file_.directory_capacity = DataCapacity;
            }
        }
        else {
            file_.chain = node->u.file.chain;
            file_.position_index = node->u.file.position_index;
            file_.record_index = node->u.file.record_index;
        }

        // If we're being asked to load attributes.
        if (file_cfg.nattrs > 0) {
            if (node->u.file.attributes.valid()) {
                attribute_storage_type attributes_storage{ pc() };
                auto err = attributes_storage.read(node->u.file.attributes, id, file_cfg);
                if (err < 0) {
                    return err;
                }
            }
        }

        return 0;
    });
    if (err < 0) {
        return err;
    }

    if (deleted) {
        phydebugf("found, deleted");
        return 0;
    }

    phydebugf("found");

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

    logged_task lt{ "dir-tree-file-data" };

    auto err = flush([&](dir_node_type *node) -> int32_t {
        memcpy(node->data + position, buffer, size);
        node->u.file.directory_size = position + size;

        return 0;
    });
    if (err < 0) {
        return err;
    }

    return size;
}

int32_t directory_tree::file_chain(file_id_t id, head_tail_t chain) {
    assert(file_.id == id);

    logged_task lt{ "dir-tree-file-chain" };

    auto err = flush([&](dir_node_type *node) -> int32_t {
        assert(!node->u.file.chain.valid());

        node->u.file.directory_size = 0;
        node->u.file.chain = chain;

        file_.chain = node->u.file.chain;

        return 1;
    });
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t directory_tree::file_attributes(file_id_t id, open_file_attribute *attributes, size_t nattrs) {
    assert(file_.id == id);

    logged_task lt{ "dir-tree-file-attrs" };

    auto err = flush([&](dir_node_type *node) -> int32_t {
        auto attributes_ptr = node->u.file.attributes;

        attribute_storage_type attributes_storage{ pc() };
        auto err = attributes_storage.update(attributes_ptr, id, attributes, nattrs);
        if (err < 0) {
            return err;
        }

        if (node->u.file.attributes != attributes_ptr) {
            node->u.file.attributes = attributes_ptr;
            return 1;
        }

        return 0;
    });
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t directory_tree::file_trees(file_id_t id, tree_ptr_t position_index, tree_ptr_t record_index) {
    assert(file_.id == id);

    logged_task lt{ "dir-tree-file-trees" };

    auto err = flush([&](dir_node_type *node) -> int32_t {
        node->u.file.position_index = position_index;
        node->u.file.record_index = record_index;

        return 1;
    });
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t directory_tree::read(file_id_t id, io_writer &writer) {
    assert(file_.id == id);

    logged_task lt{ "dir-tree-read" };

    dir_node_type node;
    auto err = tree_.find(file_.id, &node, &file_node_ptr_);
    if (err < 0) {
        return err;
    }

    if (node.u.file.directory_size == 0) {
        return 0;
    }

    err = writer.write(node.data, node.u.file.directory_size);
    if (err < 0) {
        return err;
    }

    return err;
}

} // namespace phylum
