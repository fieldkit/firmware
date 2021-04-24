#pragma once

#include "phylum.h"
#include "entries.h"
#include "tree_sector.h"
#include "directory.h"
#include "sector_chain.h"
#include "data_chain.h"

namespace phylum {

template <size_t Size> struct PHY_PACKED attribute_value_t {
    uint8_t data[Size];

    attribute_value_t() {
    }

    attribute_value_t(uint32_t value) {
        memset(data, 0, sizeof(data));
        memcpy(data, &value, sizeof(uint32_t));
    }

    attribute_value_t(void const *buffer, size_t size) {
        assert(size <= Size);
        memset(data, 0, sizeof(data));
        memcpy(data, buffer, size);
    }
};

class directory_tree : public directory {
public:
    static constexpr size_t DataCapacity = 128;
    static constexpr size_t AttributeCapacity = 256;
    using dir_node_type = dirtree_tree_value_t<DataCapacity>;
    using dir_tree_type = tree_sector<uint32_t, dir_node_type, 4>;
    using attr_node_type = attribute_value_t<AttributeCapacity>;
    using attr_tree_type = tree_sector<uint32_t, attr_node_type, 15>;

private:
    working_buffers *buffers_{ nullptr };
    sector_map *sectors_{ nullptr };
    sector_allocator *allocator_{ nullptr };
    dir_tree_type tree_;
    found_file file_;
    // TODO Consider dynamically allocating this. Very stack heavy for
    // dirtree entries.
    dir_node_type node_;

public:
    directory_tree(phyctx pc, tree_ptr_t tree)
        : buffers_(&pc.buffers_), sectors_(&pc.sectors_), allocator_(&pc.allocator_), tree_(pc, tree, "dir-tree") {
    }

    directory_tree(phyctx pc, dhara_sector_t root)
        : buffers_(&pc.buffers_), sectors_(&pc.sectors_), allocator_(&pc.allocator_), tree_(pc, tree_ptr_t{ root, root }, "dir-tree") {
    }

    virtual ~directory_tree() {
    }

public:
    friend class file_appender;

    friend class file_reader;

    int32_t mount() override;

    int32_t format() override;

    int32_t log() {
        return tree_.log();
    }

    int32_t touch(const char *name) override;

    template<typename TreeType>
    int32_t touch_indexed(const char *name) {
        auto id = make_file_id(name);

        node_ = {};
        node_.u.file = dirtree_file_t(name);

        data_chain data_chain{ pc(), head_tail_t{} };
        auto err = data_chain.create_if_necessary();
        if (err < 0) {
            return err;
        }

        node_.u.file.directory_size = 0;
        node_.u.file.chain = data_chain.chain();

        auto position_index_sector = allocator_->allocate();
        auto position_index_tree = tree_ptr_t{ position_index_sector };
        TreeType position_index{ pc(), position_index_tree, "posidx" };
        err = position_index.create();
        if (err < 0) {
            return err;
        }

        node_.u.file.position_index = position_index_tree;

        auto record_index_sector = allocator_->allocate();
        auto record_index_tree = tree_ptr_t{ record_index_sector };
        TreeType record_index{ pc(), record_index_tree, "recidx" };
        err = record_index.create();
        if (err < 0) {
            return err;
        }

        node_.u.file.record_index = record_index_tree;

        file_ = {};
        file_.id = id;
        file_.chain = node_.u.file.chain;
        file_.position_index = node_.u.file.position_index;
        file_.record_index = node_.u.file.record_index;

        err = tree_.add(id, node_);
        if (err < 0) {
            return err;
        }

        alogf(LogLevels::INFO, "phylum",
              "touch-indexed '%s' data-chain=%" PRIu32 " pos-idx=%" PRIu32 " rec-idx=%" PRIu32, name, data_chain.head(),
              position_index_sector, record_index_sector);

        return 0;
    }


    int32_t unlink(const char *name) override;

    int32_t find(const char *name, open_file_config file_cfg) override;

    found_file open() override;

    tree_ptr_t to_tree_ptr() const {
        return tree_.to_tree_ptr();
    }

protected:
    int32_t file_data(file_id_t id, file_size_t position, uint8_t const *buffer, size_t size) override;

    int32_t file_chain(file_id_t id, head_tail_t chain) override;

    int32_t file_attributes(file_id_t id, open_file_attribute *attributes, size_t nattrs) override;

    int32_t file_trees(file_id_t id, tree_ptr_t position_index, tree_ptr_t record_index) override;

    int32_t read(file_id_t id, std::function<int32_t(read_buffer)> data_fn) override;

private:
    int32_t flush();

    phyctx pc() {
        return phyctx{ *buffers_, *sectors_, *allocator_ };
    }

};

}
