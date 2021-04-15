#pragma once

#include "data_chain.h"
#include "simple_buffer.h"
#include "directory.h"

namespace phylum {

class file_reader {
private:
    directory *directory_{ nullptr };
    found_file file_;
    simple_buffer buffer_;
    data_chain data_chain_;
    file_size_t position_{ 0 };

public:
    file_reader(sector_chain &other, directory *directory, found_file file);

    file_reader(working_buffers &buffers, sector_map &sectors, sector_allocator &allocator, directory *directory, found_file file);

    virtual ~file_reader();

public:
    int32_t read(uint8_t *data, size_t size);

    int32_t close();

    uint32_t u32(uint8_t type);

    file_size_t position() const {
        return position_;
    }

private:
    bool has_chain() {
        return data_chain_.valid();
    }
};

} // namespace phylum
