#pragma once

#include "sector_chain.h"

namespace phylum {

constexpr size_t MaximumNullReadSize = 65 * 1024;

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
    data_chain(phyctx pc, head_tail_t chain, const char *prefix = "dc")
        : sector_chain(pc, chain, prefix) {
    }

    virtual ~data_chain() {
    }

public:
    int32_t write(uint8_t const *data, size_t size);
    int32_t read(uint8_t *data, size_t size);
    int32_t read_delimiter(uint32_t *delimiter);
    int32_t seek_sector(dhara_sector_t new_sector, file_size_t position_at_start_of_sector, file_size_t desired_position);
    int32_t skip_bytes(file_size_t bytes);
    int32_t skip_records(record_number_t number_records);
    file_size_t total_bytes();

public:
    data_chain_cursor cursor() const {
        if (sector() == InvalidSector) {
            return data_chain_cursor{ head(), position_, position_at_start_of_sector_ };
        }
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
