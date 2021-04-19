#pragma once

#undef constrain

#include <phylum_fs.h>

#include "hal/memory.h"

namespace fk {

class standard_page_working_buffers : public phylum::working_buffers {
private:
    uint8_t *memory_{ nullptr };

public:
    standard_page_working_buffers(size_t buffer_size);

    virtual ~standard_page_working_buffers();
};

class PhylumFlashMemory : public phylum::flash_memory {
private:
    DataMemory *target_{ nullptr };

public:
    PhylumFlashMemory(DataMemory *target);

public:
    size_t block_size() override;
    size_t number_blocks() override;
    size_t page_size() override;
    int32_t erase(uint32_t address, uint32_t length) override;
    int32_t write(uint32_t address, uint8_t const *data, size_t size) override;
    int32_t read(uint32_t address, uint8_t *data, size_t size) override;
    int32_t copy_page(uint32_t source, uint32_t destiny, size_t size) override;
};

class Phylum {
private:
    size_t sector_size_;
    PhylumFlashMemory memory_;
    standard_page_working_buffers buffers_{ sector_size_ };
    phylum::dhara_sector_map sectors_{ buffers_, memory_ };
    phylum::sector_allocator allocator_{ sectors_ };

public:
    Phylum(DataMemory *data_memory);

public:
    bool sync();

};

}
