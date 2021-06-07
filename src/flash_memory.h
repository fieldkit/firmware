#pragma once

#include "phylum.h"

namespace phylum {

class flash_memory {
public:
    virtual size_t block_size() = 0;
    virtual size_t number_blocks() = 0;
    virtual size_t page_size() = 0;
    virtual int32_t erase(uint32_t address, uint32_t length) = 0;
    virtual int32_t write(uint32_t address, uint8_t const *data, size_t size) = 0;
    virtual int32_t read(uint32_t address, uint8_t *data, size_t size) = 0;
    virtual int32_t copy_page(uint32_t source, uint32_t destiny, size_t size) = 0;
};

} // namespace phylum
