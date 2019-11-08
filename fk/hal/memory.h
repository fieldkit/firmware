#pragma once

#include "common.h"
#include "config.h"
#include "platform.h"

namespace fk {

struct FlashGeometry {
    uint32_t page_size;
    uint32_t block_size;
    uint32_t nblocks;
    uint32_t total_size;

    uint32_t beginning() const {
        return 0;
    }

    uint32_t end() const {
        return total_size;
    }

    uint32_t number_of_blocks() const {
        return total_size / block_size;
    }

    uint32_t remaining_in_page(uint32_t address) {
        return page_size - (address % page_size);
    }

    uint32_t remaining_in_block(uint32_t address) {
        return block_size - (address % block_size);
    }

    uint32_t truncate_to_block(uint32_t address) {
        uint32_t block = (address / block_size);
        return block * block_size;
    }

    uint32_t start_of_block(uint32_t address) {
        return (address / block_size) * block_size;
    }

    bool is_start_of_block(uint32_t address) {
        return address % block_size == 0;
    }

    bool is_start_of_block_or_header(uint32_t address, size_t block_header_size) {
        auto sob = start_of_block(address);
        return sob == address || sob + block_header_size == address;
    }

    bool spans_block(uint32_t address, uint32_t length) {
        return (address / block_size) != ((address + length) / block_size);
    }

    bool is_address_valid(uint32_t address) {
        return address >= 0 && address < total_size;
    }

    uint32_t partial_write_boundary_before(uint32_t address) {
        if (address < 512) {
            return 0;
        }
        auto padding = address % 512;
        return padding == 0 ? address : address - padding;
    }

    uint32_t partial_write_boundary_after(uint32_t address) {
        auto padding = address % 512;
        return padding == 0 ? address : address + (512 - padding);
    }
};

class DataMemory {
public:
    virtual bool begin() = 0;

    virtual FlashGeometry geometry() const = 0;

    virtual int32_t read(uint32_t address, uint8_t *data, size_t length) = 0;

    virtual int32_t write(uint32_t address, uint8_t const *data, size_t length) = 0;

    virtual int32_t erase_block(uint32_t address) = 0;

    virtual int32_t flush() = 0;

    bool available() {
        return geometry().total_size > 0;
    }

};

class BankedDataMemory : public DataMemory {
private:
    DataMemory **memories_;
    size_t size_;
    FlashGeometry geometry_;

public:
    BankedDataMemory(DataMemory **memories, size_t size);

public:
    bool begin() override;

    FlashGeometry geometry() const override;

    int32_t read(uint32_t address, uint8_t *data, size_t length) override;

    int32_t write(uint32_t address, uint8_t const *data, size_t length) override;

    int32_t erase_block(uint32_t address) override;

    int32_t flush() override;

};

class MemoryFactory {
public:
    constexpr static size_t NumberOfDataMemoryBanks = FK_MAXIMUM_NUMBER_OF_MEMORY_BANKS;

public:
    static DataMemory **get_data_memory_banks();
    static DataMemory *get_data_memory();
    static DataMemory *get_qspi_memory();
};

}
