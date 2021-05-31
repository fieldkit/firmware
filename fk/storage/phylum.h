#pragma once

#undef constrain

#include <phylum_fs.h>

#include "hal/memory.h"
#include "storage/phylum_flash_memory.h"

namespace fk {

using directory_type = phylum::directory_tree;
using index_tree_type = phylum::tree_sector<uint32_t, uint32_t, 201>;
using file_ops_type = phylum::file_ops<directory_type, index_tree_type>;

class standard_page_working_buffers : public phylum::working_buffers {
private:
    static constexpr size_t NumberOfPages = 4;
    uint8_t *pages_[NumberOfPages]{ nullptr, nullptr, nullptr, nullptr };

public:
    standard_page_working_buffers(size_t buffer_size);
    virtual ~standard_page_working_buffers();

public:
    bool lend_pages();
    void free_pages();

};

class Phylum {
private:
    size_t sector_size_;
    PhylumFlashMemory memory_;
    standard_page_working_buffers buffers_{ sector_size_ };
    phylum::noop_page_cache page_cache_;
    // phylum::simple_page_cache page_cache_{ buffers_.allocate(sector_size_) };
    phylum::dhara_sector_map sectors_{ buffers_, memory_, &page_cache_ };
    phylum::sector_allocator allocator_{ sectors_ };

public:
    Phylum(DataMemory *data_memory);

public:
    phylum::phyctx pc() {
        return phylum::phyctx{ buffers_, sectors_, allocator_ };
    }

public:
    bool begin(bool force_create);
    bool format();
    bool mount();
    bool sync();

};

}

#include "storage/phylum_data_file.h"
