#pragma once

#include "record_chain.h"

namespace phylum {

class free_sectors_chain : public record_chain {
public:
    free_sectors_chain(phyctx pc, head_tail_t chain);

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
