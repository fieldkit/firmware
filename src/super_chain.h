#pragma once

#include "record_chain.h"

namespace phylum {

class super_chain : public record_chain {
private:
    tree_ptr_t directory_tree_;

public:
    super_chain(phyctx pc, dhara_sector_t head) : record_chain(pc, head_tail_t{ head, InvalidSector }, "super-chain") {
    }

public:
    int32_t mount();

    int32_t format();

    int32_t update(tree_ptr_t directory_tree);

public:
    tree_ptr_t directory_tree() const {
        return directory_tree_;
    }

protected:
    int32_t write_header(page_lock &page_lock) override;

};

} // namespace phylum
