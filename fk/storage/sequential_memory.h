#pragma once

#include <stdlib.h>
#include <memory>

#include "common.h"
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
    SequentialWrapper(DataMemory *target) : target_(target) {
    }

public:
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

    int32_t flush() {
        return target_.flush();
    }
};

template <class T>
using unique_ptr_freed = std::unique_ptr<T, decltype(&free)>;

class BufferedPageMemory : public DataMemory {
private:
    DataMemory *target_;
    unique_ptr_freed<uint8_t> buffer_;
    uint32_t cached_{ UINT32_MAX };
    bool dirty_{ false };

public:
    BufferedPageMemory(DataMemory *target);
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
