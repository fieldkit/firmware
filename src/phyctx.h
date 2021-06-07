#pragma once

#include "working_buffers.h"
#include "sector_map.h"
#include "sector_allocator.h"

namespace phylum {

class phyctx {
private:
    working_buffers &buffers_;
    sector_map &sectors_;
    sector_allocator &allocator_;

public:
    phyctx(working_buffers &buffers, sector_map &sectors, sector_allocator &allocator)
        : buffers_(buffers), sectors_(sectors), allocator_(allocator) {
    }

    virtual ~phyctx() {
    }

public:
    friend class sector_chain;
    friend class file_appender;
    friend class file_reader;

    template <typename KEY, typename VALUE, size_t Size>
    friend class tree_sector;
    friend class directory_tree;

    friend class tree_attribute_storage;
    friend class flat_attribute_storage;
};

} // namespace phylum
