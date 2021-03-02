#pragma once

#include "hal/memory.h"

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

class StatisticsMemory : public DataMemory {
private:
    DataMemory *target_;
    MemoryStatistics statistics_;

public:
    StatisticsMemory(DataMemory *target) : target_(target) {
    }

public:
    bool begin() override;

    FlashGeometry geometry() const override;

    int32_t read(uint32_t address, uint8_t *data, size_t length, MemoryReadFlags falgs) override;

    int32_t write(uint32_t address, const uint8_t *data, size_t length, MemoryWriteFlags flags) override;

    int32_t erase(uint32_t address, size_t length) override;

    int32_t erase_block(uint32_t address) override;

    int32_t flush() override;

    MemoryStatistics &statistics();

    void log_statistics() {
        statistics_.log();
    }

};

}
