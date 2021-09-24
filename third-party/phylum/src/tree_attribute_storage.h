#pragma once

#include "phylum.h"
#include "entries.h"
#include "tree_sector.h"
#include "directory.h"
#include "sector_chain.h"
#include "data_chain.h"

namespace phylum {

template <size_t Size>
struct PHY_PACKED attribute_value_t {
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

class tree_attribute_storage {
public:
    static constexpr size_t AttributeCapacity = 256;
    using attr_node_type = attribute_value_t<AttributeCapacity>;
    using attr_tree_type = tree_sector<uint32_t, attr_node_type, 15>;

private:
    working_buffers *buffers_{ nullptr };
    sector_map *sectors_{ nullptr };
    sector_allocator *allocator_{ nullptr };

public:
    tree_attribute_storage(phyctx pc) : buffers_(&pc.buffers_), sectors_(&pc.sectors_), allocator_(&pc.allocator_) {
    }

public:
    int32_t read(tree_ptr_t &ptr, file_id_t id, open_file_config file_cfg);
    int32_t update(tree_ptr_t &ptr, file_id_t id, open_file_attribute *attributes, size_t nattrs);
};

} // namespace phylum
