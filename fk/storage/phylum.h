#pragma once

#undef constrain

#include <phylum_fs.h>

#include "hal/memory.h"
#include "storage/phylum_flash_memory.h"
#include "pool.h"

namespace fk {

using directory_type = phylum::directory_tree;
using index_tree_type = phylum::tree_sector<uint32_t, uint32_t, 201>;
using file_ops_type = phylum::file_ops<directory_type, index_tree_type>;

class standard_page_buffer_memory : public phylum::buffer_memory {
private:
    Pool *pool_{ nullptr };

    struct page_t {
        size_t size{ 0 };
        size_t position{ 0 };
        uint8_t *ptr{ nullptr };
        page_t *np{ nullptr };
    };

    page_t *pages_{ nullptr };

public:
    standard_page_buffer_memory(Pool *pool);
    virtual ~standard_page_buffer_memory();

public:
	void *alloc_memory(size_t size) override;
	void free_memory(void *ptr) override;
	void *alloc_page(size_t size) override;
	void free_page(void *ptr) override;

};

class Phylum {
public:
    static constexpr size_t WorkingBuffersSize = 8;

private:
    PhylumFlashMemory memory_;
    Pool *pool_{ nullptr };
    size_t sector_size_;
    standard_page_buffer_memory buffer_memory_{ pool_ };
    phylum::working_buffers buffers_{ &buffer_memory_, sector_size_, WorkingBuffersSize };
    phylum::noop_page_cache page_cache_;
    // phylum::simple_page_cache page_cache_{ buffers_.allocate(sector_size_) };
    phylum::dhara_sector_map sectors_{ buffers_, memory_, &page_cache_ };
    phylum::sector_allocator allocator_{ sectors_ };

public:
    Phylum(DataMemory *data_memory, Pool &pool);

public:
    phylum::phyctx pc() {
        return phylum::phyctx{ buffers_, sectors_, allocator_ };
    }

    uint32_t bytes_used();

public:
    bool begin(bool force_create);
    bool format();
    bool mount();
    bool sync();

};

}

#include "storage/phylum_data_file.h"
