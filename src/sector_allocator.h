#pragma once

#include "dhara_map.h"

namespace phylum {

class sector_allocator {
private:
    sector_map &sectors_;
    dhara_sector_t counter_{ 0 };

public:
    sector_allocator(sector_map &sectors) : sectors_(sectors) {
    }

public:
    int32_t begin() {
        counter_ = sectors_.size() + 1;

        return 0;
    }

    virtual dhara_sector_t allocate() {
        // TODO We should also maintain a free sector collection. We
        // can do this by just keeping a sector chain with their
        // numbers.
        return counter_++;
    }

    dhara_sector_t allocated() {
        return counter_;
    }

};

} // namespace phylum
