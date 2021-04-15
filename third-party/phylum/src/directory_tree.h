#pragma once

#include "phylum.h"
#include "entries.h"
#include "tree_sector.h"
#include "directory.h"
#include "sector_chain.h"

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
private:
    static constexpr size_t DataCapacity = 128 + 29;
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
    dir_node_type node_;

public:
    directory_tree(working_buffers &buffers, sector_map &sectors, sector_allocator &allocator, dhara_sector_t sector)
        : buffers_(&buffers), sectors_(&sectors), allocator_(&allocator), tree_(buffers, sectors, allocator, tree_ptr_t{ sector, sector }, "dir-chain") {
    }

    directory_tree(working_buffers &buffers, sector_map &sectors, sector_allocator &allocator, tree_ptr_t tree)
        : buffers_(&buffers), sectors_(&sectors), allocator_(&allocator), tree_(buffers, sectors, allocator, tree, "dir-chain") {
    }

    virtual ~directory_tree() {
    }

public:
    friend class file_appender;

    friend class file_reader;

    int32_t mount() override;

    int32_t format() override;

    int32_t touch(const char *name) override;

    int32_t unlink(const char *name) override;

    int32_t find(const char *name, open_file_config file_cfg) override;

    found_file open() override;

protected:
    int32_t file_data(file_id_t id, uint8_t const *buffer, size_t size) override;

    int32_t file_chain(file_id_t id, head_tail_t chain) override;

    int32_t file_attributes(file_id_t id, open_file_attribute *attributes, size_t nattrs) override;

    int32_t read(file_id_t id, std::function<int32_t(read_buffer)> data_fn) override;

private:
    int32_t flush();

};

}
