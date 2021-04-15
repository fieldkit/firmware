#pragma once

#include "sector_chain.h"

namespace phylum {

enum seek_reference {
    Start,
    End,
};

struct data_chain_cursor {
    dhara_sector_t sector{ 0 };
    file_size_t position{ 0 };
    file_size_t position_at_start_of_sector{ 0 };

    data_chain_cursor() {
    }

    data_chain_cursor(dhara_sector_t sector) : sector(sector), position(0), position_at_start_of_sector(0) {
    }

    data_chain_cursor(dhara_sector_t sector, file_size_t position, file_size_t position_at_start_of_sector)
        : sector(sector), position(position), position_at_start_of_sector(position_at_start_of_sector) {
    }
};

class data_chain : public sector_chain {
private:
    head_tail_t chain_{ };
    file_size_t position_{ 0 };
    file_size_t position_at_start_of_sector_{ 0 };

public:
    data_chain(working_buffers &buffers, sector_map &sectors, sector_allocator &allocator, head_tail_t chain, const char *prefix)
        : sector_chain(buffers, sectors, allocator, chain, prefix) {
    }

    data_chain(sector_chain &other, head_tail_t chain) : sector_chain(other, chain, "data"), chain_(chain) {
    }

    data_chain(sector_chain &other, head_tail_t chain, const char *prefix)
        : sector_chain(other, chain, prefix), chain_(chain) {
    }

    virtual ~data_chain() {
    }

public:
    int32_t write(uint8_t const *data, size_t size);
    int32_t read(uint8_t *data, size_t size);
    uint32_t total_bytes();
    int32_t seek(file_size_t position, seek_reference reference);

public:
    data_chain_cursor cursor() {
        return data_chain_cursor{ sector(), position_, position_at_start_of_sector_ };
    }

protected:
    int32_t write_header(page_lock &page_lock) override;

    int32_t seek_end_of_buffer(page_lock &page_lock) override;

    int32_t write_chain(std::function<int32_t(write_buffer, bool&)> data_fn);

    int32_t read_chain(std::function<int32_t(read_buffer)> data_fn);

    int32_t constrain();

};

} // namespace phylum
