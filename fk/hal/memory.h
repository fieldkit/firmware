#pragma once

#include "common.h"
#include "config.h"
#include "platform.h"

namespace fk {

struct FlashGeometry {
    static constexpr uint32_t SectorSize = 512;

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

    uint32_t remaining_in_page(uint32_t address) const {
        return page_size - (address % page_size);
    }

    uint32_t remaining_in_block(uint32_t address) const {
        return block_size - (address % block_size);
    }

    uint32_t truncate_to_block(uint32_t address) const {
        uint32_t block = (address / block_size);
        return block * block_size;
    }

    uint32_t start_of_block(uint32_t address) const {
        return (address / block_size) * block_size;
    }

    bool is_start_of_block(uint32_t address) const {
        return address % block_size == 0;
    }

    bool is_start_of_block_or_header(uint32_t address, size_t block_header_size) const {
        auto sob = start_of_block(address);
        return sob == address || sob + block_header_size == address;
    }

    bool spans_block(uint32_t address, uint32_t length) const {
        return (address / block_size) != ((address + length) / block_size);
    }

    bool is_address_valid(uint32_t address) const {
        return address >= 0 && address < total_size;
    }

    uint32_t partial_write_boundary_before(uint32_t address) const {
        if (address < SectorSize) {
            return 0;
        }
        auto padding = address % SectorSize;
        return padding == 0 ? address : address - padding;
    }

    uint32_t partial_write_boundary_after(uint32_t address) const {
        auto padding = address % SectorSize;
        return padding == 0 ? address : address + (SectorSize - padding);
    }

    uint32_t sector_size() const {
        return SectorSize;
    }
};

enum class MemoryReadFlags {
    None,
};

enum class MemoryWriteFlags {
    None,
};

class DataMemory {
public:
    virtual bool begin() = 0;

    virtual FlashGeometry geometry() const = 0;

    virtual int32_t read(uint32_t address, uint8_t *data, size_t length, MemoryReadFlags flags) = 0;

    virtual int32_t write(uint32_t address, uint8_t const *data, size_t length, MemoryWriteFlags flags) = 0;

    virtual int32_t erase_block(uint32_t address) = 0;

    virtual int32_t flush() = 0;

    int32_t read(uint32_t address, uint8_t *data, size_t length) {
        return read(address, data, length, MemoryReadFlags::None);
    }

    int32_t write(uint32_t address, uint8_t const *data, size_t length) {
        return write(address, data, length, MemoryWriteFlags::None);
    }

    bool available() {
        return geometry().total_size > 0;
    }

};

class EmptyMemory : public DataMemory {
public:
    bool begin() override {
        return false;
    }

    FlashGeometry geometry() const override {
        return { };
    }

    int32_t read(uint32_t address, uint8_t *data, size_t length, MemoryReadFlags flags) override {
        FK_ASSERT(false);
        return (int32_t)Error::IO;
    }

    int32_t write(uint32_t address, uint8_t const *data, size_t length, MemoryWriteFlags flags) override {
        FK_ASSERT(false);
        return (int32_t)Error::IO;
    }

    int32_t erase_block(uint32_t address) override {
        FK_ASSERT(false);
        return (int32_t)Error::IO;
    }

    int32_t flush() override {
        return 0;
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

    int32_t read(uint32_t address, uint8_t *data, size_t length, MemoryReadFlags flags) override;

    int32_t write(uint32_t address, uint8_t const *data, size_t length, MemoryWriteFlags flags) override;

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
