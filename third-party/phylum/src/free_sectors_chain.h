#pragma once

#include "record_chain.h"

namespace phylum {

class free_sectors_chain : public record_chain {
public:
    free_sectors_chain(working_buffers &buffers, sector_map &sectors, sector_allocator &allocator, head_tail_t chain, const char *prefix);

    free_sectors_chain(sector_chain &other, head_tail_t chain, const char *prefix);

    virtual ~free_sectors_chain();

public:
    int32_t add_chain(dhara_sector_t head);
    int32_t add_tree(tree_ptr_t tree);
    int32_t dequeue(dhara_sector_t *sector);

private:
    int32_t add_free_sectors(free_sectors_t record);
    int32_t write_header(page_lock &page_lock) override;
    int32_t seek_end_of_buffer(page_lock &page_lock) override;

};

}
