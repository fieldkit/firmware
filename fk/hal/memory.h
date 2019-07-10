#pragma once

#include "common.h"

namespace fk {

typedef struct flash_geometry_t {
    uint32_t page_size;
    uint32_t block_size;
    uint32_t nblocks;
    uint32_t total_size;
} flash_geometry_t;

class DataMemory {
public:
    virtual bool begin() = 0;

    virtual flash_geometry_t geometry() const = 0;

    virtual bool read(uint32_t address, uint8_t *data, uint32_t length) = 0;

    virtual bool write(uint32_t address, const uint8_t *data, uint32_t length) = 0;

    virtual bool erase_block(uint32_t address) = 0;

    bool available() {
        return geometry().total_size > 0;
    }

};

class BankedDataMemory : public DataMemory {
private:
    DataMemory **memories_;
    size_t size_;
    flash_geometry_t geometry_;

public:
    BankedDataMemory(DataMemory **memories, size_t size);

public:
    bool begin() override;

    flash_geometry_t geometry() const override;

    bool read(uint32_t address, uint8_t *data, uint32_t length) override;

    bool write(uint32_t address, const uint8_t *data, uint32_t length) override;

    bool erase_block(uint32_t address) override;

};

class SequentialMemory {
private:
    DataMemory *memory_;

public:
    SequentialMemory(DataMemory *memory);

public:
    uint32_t read(uint32_t address, uint8_t *data, uint32_t length);
    uint32_t write(uint32_t address, uint8_t *data, uint32_t length);

};

class MemoryFactory {
public:
    constexpr static size_t NumberOfDataMemoryBanks = 4;

public:
    static DataMemory **get_data_memory_banks();
    static DataMemory *get_data_memory();
};

}
