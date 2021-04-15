#pragma once

#include <data_chain.h>

namespace phylum {

class integer_chain : public data_chain {
public:
    integer_chain(sector_chain &other, head_tail_t chain) : data_chain(other, chain, "integers") {
    }

    virtual ~integer_chain() {
    }

public:
    int32_t write(uint32_t const *values, size_t length);
    int32_t read(uint32_t *values, size_t length);

protected:
    int32_t write_header(page_lock &page_lock) override;
    int32_t seek_end_of_buffer(page_lock &page_lock) override;
};

} // namespace phylum
