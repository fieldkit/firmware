#pragma once

#include "phylum.h"
#include "sector_chain.h"
#include "sector_map.h"
#include "working_buffers.h"
#include "phyctx.h"
#include "directory.h"

namespace phylum {

class flat_attribute_storage {
public:
    static constexpr size_t AttributeCapacity = 4000;

private:
    working_buffers *buffers_{ nullptr };
    sector_map *sectors_{ nullptr };
    sector_allocator *allocator_{ nullptr };

public:
    flat_attribute_storage(phyctx pc);

public:
    int32_t read(tree_ptr_t &ptr, file_id_t id, open_file_config file_cfg);
    int32_t update(tree_ptr_t &ptr, file_id_t id, open_file_attribute *attributes, size_t nattrs);

private:
    phyctx pc() const {
        return phyctx{ *buffers_, *sectors_, *allocator_ };
    }

};

} // namespace phylum
