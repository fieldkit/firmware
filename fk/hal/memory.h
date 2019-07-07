#pragma once

namespace fk {

typedef struct flash_geometry_t {
    uint32_t page_size;
    uint32_t block_size;
    uint32_t nblocks;
} flash_geometry_t;

class DataMemory {
public:
    virtual bool begin() = 0;

    virtual flash_geometry_t get_geometry() const = 0;

    virtual bool read(uint32_t address, uint8_t *data, uint32_t length) = 0;

    virtual bool write(uint32_t address, const uint8_t *data, uint32_t length) = 0;

    virtual bool erase_block(uint32_t address) = 0;

};

}
