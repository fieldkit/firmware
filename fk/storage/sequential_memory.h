#pragma once

#include "hal/memory.h"

namespace fk {

class SequentialMemory {
private:
    DataMemory *target_;

public:
    SequentialMemory(DataMemory *target);

public:
    int32_t read(uint32_t address, uint8_t *data, size_t length);
    int32_t write(uint32_t address, uint8_t const *data, size_t length);
    int32_t flush();

};

class CacheSinglePageMemory : public DataMemory {
private:
    DataMemory *target_;
    uint8_t *buffer_;
    uint32_t cached_{ UINT32_MAX };
    bool dirty_{ false };

public:
    CacheSinglePageMemory(DataMemory *target);
    virtual ~CacheSinglePageMemory();

public:
    bool begin() override;
    FlashGeometry geometry() const override;
    int32_t read(uint32_t address, uint8_t *data, size_t length, MemoryReadFlags flags) override;
    int32_t write(uint32_t address, uint8_t const *data, size_t length, MemoryWriteFlags flags) override;
    int32_t erase_block(uint32_t address) override;
    int32_t flush();

};

}
