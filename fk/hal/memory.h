#pragma once

#include "common.h"
#include "config.h"

namespace fk {

typedef struct MemoryStatistics {
    uint32_t nreads{ 0 };
    uint32_t nwrites{ 0 };
    uint32_t nerases{ 0 };
    uint32_t bytes_read{ 0 };
    uint32_t bytes_wrote{ 0 };

    void add_read(uint32_t bytes) {
        nreads++;
        bytes_read += bytes;
    }

    void add_write(uint32_t bytes) {
        nwrites++;
        bytes_wrote += bytes;
    }

    void add(MemoryStatistics s);

    void log() const;
} MemoryStatistics;

typedef struct flash_geometry_t {
    uint32_t page_size;
    uint32_t block_size;
    uint32_t nblocks;
    uint32_t total_size;

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

    bool start_of_block(uint32_t address) {
        return address % block_size == 0;
    }

    bool spans_block(uint32_t address, uint32_t length) {
        return (address / block_size) != ((address + length) / block_size);
    }

    bool is_address_valid(uint32_t address) {
        return address >= 0 && address < total_size;
    }
} flash_geometry_t;

class DataMemory {
public:
    virtual bool begin() = 0;

    virtual flash_geometry_t geometry() const = 0;

    virtual size_t read(uint32_t address, uint8_t *data, size_t length) = 0;

    virtual size_t write(uint32_t address, const uint8_t *data, size_t length) = 0;

    virtual size_t erase_block(uint32_t address) = 0;

    virtual size_t flush() = 0;

    bool available() {
        return geometry().total_size > 0;
    }

};

class StatisticsMemory : public DataMemory {
private:
    DataMemory *target_;
    MemoryStatistics statistics_;

public:
    StatisticsMemory(DataMemory *target) : target_(target) {
    }

public:
    bool begin() override;

    flash_geometry_t geometry() const override;

    size_t read(uint32_t address, uint8_t *data, size_t length) override;

    size_t write(uint32_t address, const uint8_t *data, size_t length) override;

    size_t erase_block(uint32_t address) override;

    size_t flush() override;

    MemoryStatistics &statistics();

    void log_statistics() {
        statistics_.log();
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

    size_t read(uint32_t address, uint8_t *data, size_t length) override;

    size_t write(uint32_t address, const uint8_t *data, size_t length) override;

    size_t erase_block(uint32_t address) override;

    size_t flush() override;

};

class SequentialMemory {
private:
    DataMemory *memory_;

public:
    SequentialMemory(DataMemory *memory);

public:
    size_t read(uint32_t address, uint8_t *data, size_t length);
    size_t write(uint32_t address, uint8_t *data, size_t length);
    size_t flush();

};

class MemoryFactory {
public:
    constexpr static size_t NumberOfDataMemoryBanks = FK_MAXIMUM_NUMBER_OF_MEMORY_BANKS;

public:
    static DataMemory **get_data_memory_banks();
    static DataMemory *get_data_memory();
};

}
