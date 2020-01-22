#pragma once

#include <stdlib.h>
#include <memory>

#include "common.h"
#include "pool.h"
#include "memory.h"
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

template<class T>
class SequentialWrapper {
private:
    T target_;

public:
    SequentialWrapper(DataMemory *target, Pool &pool) : target_(target, pool) {
    }

public:
    FlashGeometry geometry() const {
        return target_.geometry();
    }

    int32_t read(uint32_t address, uint8_t *data, size_t length) {
        size_t nbytes = 0;

        auto g = target_.geometry();
        auto p = data;
        auto remaining = length;

        FK_ASSERT(g.is_address_valid(address));

        auto rib = g.remaining_in_block(address);

        FK_ASSERT(length <= rib);

        while (nbytes != length) {
            auto left = g.remaining_in_page(address);
            auto reading = std::min<size_t>(remaining, left);
            if (!target_.read(address, p, reading, MemoryReadFlags::None)) {
                return nbytes;
            }

            address += reading;
            remaining -= reading;
            nbytes += reading;
            p += reading;
        }

        return nbytes;
    }

    int32_t write(uint32_t address, uint8_t const *data, size_t length) {
        size_t nbytes = 0;

        auto g = target_.geometry();
        auto p = data;
        auto remaining = length;

        FK_ASSERT(g.is_address_valid(address));

        auto rib = g.remaining_in_block(address);

        FK_ASSERT(length <= rib);

        while (nbytes != length) {
            auto left = g.remaining_in_page(address);
            auto writing = std::min<size_t>(remaining, left);
            if (!target_.write(address, p, writing, MemoryWriteFlags::None)) {
                return nbytes;
            }

            address += writing;
            remaining -= writing;
            nbytes += writing;
            p += writing;
        }

        return nbytes;
    }

    int32_t erase_block(uint32_t address) {
        return target_.erase_block(address);
    }

    int32_t flush() {
        return target_.flush();
    }
};

class BufferedPageMemory : public DataMemory {
private:
    DataMemory *target_;
    uint8_t *buffer_;
    uint32_t cached_{ UINT32_MAX };
    bool dirty_{ false };
    int16_t dirty_start_{ -1 };
    int16_t dirty_end_{ -1 };

public:
    BufferedPageMemory(DataMemory *target, Pool &pool);
    BufferedPageMemory(BufferedPageMemory &&o);
    BufferedPageMemory(BufferedPageMemory const &o) = delete;
    virtual ~BufferedPageMemory();

public:
    BufferedPageMemory &operator=(BufferedPageMemory const &o) = delete;
    BufferedPageMemory &operator=(BufferedPageMemory &&o);

public:
    bool begin() override;
    FlashGeometry geometry() const override;
    int32_t read(uint32_t address, uint8_t *data, size_t length, MemoryReadFlags flags) override;
    int32_t write(uint32_t address, uint8_t const *data, size_t length, MemoryWriteFlags flags) override;
    int32_t erase_block(uint32_t address) override;
    int32_t flush();

public:
    using DataMemory::read;
    using DataMemory::write;
};

} // namespace fk
