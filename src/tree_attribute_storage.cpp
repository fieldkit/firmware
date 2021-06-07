#include "tree_attribute_storage.h"

namespace phylum {

int32_t tree_attribute_storage::read(tree_ptr_t &ptr, file_id_t /*id*/, open_file_config file_cfg) {
    attr_tree_type tree{ phyctx{ *buffers_, *sectors_, *allocator_ }, ptr, "attrs" };
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

    return 0;
}

int32_t tree_attribute_storage::update(tree_ptr_t &ptr, file_id_t /*id*/, open_file_attribute *attributes, size_t nattrs) {
    auto attribute_size = 0u;
    for (auto i = 0u; i < nattrs; ++i) {
        assert(attributes[i].size <= AttributeCapacity);
        attribute_size += attributes[i].size;
    }

    phydebugf("saving attributes total-size=%zu", attribute_size);

    // TODO Store attributes in inline data when we can.
    auto create = false;
    if (!ptr.valid()) {
        auto allocated = allocator_->allocate();
        ptr = tree_ptr_t{ allocated, allocated };
        create = true;
    }

    attr_tree_type tree{ phyctx{ *buffers_, *sectors_, *allocator_ }, ptr, "attrs" };

    if (create) {
        auto err = tree.create();
        if (err < 0) {
            return err;
        }
    }

    for (auto i = 0u; i < nattrs; ++i) {
        auto &attr = attributes[i];
        attr_node_type node{ attr.ptr, attr.size };
        auto err = tree.add(attr.type, &node, nullptr);
        if (err < 0) {
            return err;
        }
    }

    return 0;
}

} // namespace phylum
