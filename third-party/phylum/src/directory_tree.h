#pragma once

#include "phylum.h"
#include "entries.h"
#include "tree_sector.h"
#include "directory.h"
#include "sector_chain.h"
#include "data_chain.h"
#include "tree_attribute_storage.h"
#include "flat_attribute_storage.h"

namespace phylum {

class directory_tree : public directory {
public:
    static constexpr size_t DataCapacity = 128;
    using dir_node_type = dirtree_tree_value_t<DataCapacity>;
    using dir_tree_type = tree_sector<uint32_t, dir_node_type, 4>;
    using attribute_storage_type = flat_attribute_storage;

private:
    working_buffers *buffers_{ nullptr };
    sector_map *sectors_{ nullptr };
    sector_allocator *allocator_{ nullptr };
    dir_tree_type tree_;
    tree_value_ptr_t file_node_ptr_;
    found_file file_;

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
    int32_t touch_indexed(const char *name, open_file_config file_cfg) {
        auto id = make_file_id(name);

        dir_node_type node = {};
        node.u.file = dirtree_file_t(name);

        data_chain data_chain{ pc(), head_tail_t{} };
        auto err = data_chain.create_if_necessary();
        if (err < 0) {
            return err;
        }

        node.u.file.directory_size = 0;
        node.u.file.chain = data_chain.chain();

        auto position_index_sector = allocator_->allocate();
        auto position_index_tree = tree_ptr_t{ position_index_sector };
        TreeType position_index{ pc(), position_index_tree, "pos-idx" };
        err = position_index.create();
        if (err < 0) {
            return err;
        }

        node.u.file.position_index = position_index_tree;

        auto record_index_sector = allocator_->allocate();
        auto record_index_tree = tree_ptr_t{ record_index_sector };
        TreeType record_index{ pc(), record_index_tree, "rec-idx" };
        err = record_index.create();
        if (err < 0) {
            return err;
        }

        node.u.file.record_index = record_index_tree;

        file_ = {};
        file_.id = id;
        file_.chain = node.u.file.chain;
        file_.position_index = node.u.file.position_index;
        file_.record_index = node.u.file.record_index;

        err = tree_.add(id, &node, &file_node_ptr_);
        if (err < 0) {
            return err;
        }

        if (file_cfg.nattrs > 0) {
            auto err = file_attributes(id, file_cfg.attributes, file_cfg.nattrs);
            if (err < 0) {
                return err;
            }
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

    int32_t read(file_id_t id, io_writer &writer) override;

private:
    phyctx pc() {
        return phyctx{ *buffers_, *sectors_, *allocator_ };
    }

    template<typename FlushFunction>
    int32_t flush(FlushFunction fn) {
        assert(file_node_ptr_.node.sector != InvalidSector);

        auto err = tree_.modify_in_place(file_node_ptr_, fn);
        if (err < 0) {
            return err;
        }

        return 0;
    }

};

}
