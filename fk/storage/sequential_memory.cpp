#include <algorithm>

#include "storage/sequential_memory.h"
#include "storage/storage.h"
#include "exchange.h"
#include "utilities.h"

namespace fk {

FK_DECLARE_LOGGER("memory");

SequentialMemory::SequentialMemory(DataMemory *target) : target_(target) {
}

int32_t SequentialMemory::read(uint32_t address, uint8_t *data, size_t length) {
    size_t nbytes = 0;

    auto g = target_->geometry();
    auto p = data;
    auto remaining = length;

    FK_ASSERT(g.is_address_valid(address));

    auto rib = g.remaining_in_block(address);

    FK_ASSERT(length <= rib);

    while (nbytes != length) {
        auto left = g.remaining_in_page(address);
        auto reading = std::min<size_t>(remaining, left);
        if (!target_->read(address, p, reading, MemoryReadFlags::None)) {
            return nbytes;
        }

        address += reading;
        remaining -= reading;
        nbytes += reading;
        p += reading;
    }

    return nbytes;
}

int32_t SequentialMemory::write(uint32_t address, uint8_t const *data, size_t length) {
    size_t nbytes = 0;

    auto g = target_->geometry();
    auto p = data;
    auto remaining = length;

    FK_ASSERT(g.is_address_valid(address));

    auto rib = g.remaining_in_block(address);

    FK_ASSERT(length <= rib);

    while (nbytes != length) {
        auto left = g.remaining_in_page(address);
        auto writing = std::min<size_t>(remaining, left);
        if (!target_->write(address, p, writing, MemoryWriteFlags::None)) {
            return nbytes;
        }

        address += writing;
        remaining -= writing;
        nbytes += writing;
        p += writing;
    }

    return nbytes;
}

int32_t SequentialMemory::flush() {
    return target_->flush();
}

BufferedPageMemory::BufferedPageMemory(DataMemory *target, Pool &pool)
    : target_(target), buffer_{ reinterpret_cast<uint8_t*>(pool.malloc(target_->geometry().page_size)) } {
}

BufferedPageMemory::BufferedPageMemory(BufferedPageMemory &&o) : target_(o.target_), buffer_{ exchange(o.buffer_, nullptr) } {
}

BufferedPageMemory::~BufferedPageMemory() {
}

BufferedPageMemory &BufferedPageMemory::operator=(BufferedPageMemory &&o) {
    target_ = o.target_;
    buffer_ = exchange(o.buffer_, nullptr);
    cached_ = o.cached_;
    dirty_ = o.dirty_;
    return *this;
}

bool BufferedPageMemory::begin() {
    return target_->begin();
}

FlashGeometry BufferedPageMemory::geometry() const {
    return target_->geometry();
}

int32_t BufferedPageMemory::read(uint32_t address, uint8_t *data, size_t length, MemoryReadFlags flags) {
    auto g = target_->geometry();
    auto page = address / g.page_size;
    if (cached_ == UINT32_MAX || page != cached_) {
        auto rv = target_->read(page * g.page_size, buffer_, g.page_size, flags);
        if (rv <= 0) {
            return rv;
        }
        cached_ = page;
        dirty_start_ = -1;
        dirty_end_ = -1;
    }
    auto page_offset = address % g.page_size;
    memcpy(data, buffer_ + page_offset, length);
    return length;
}

int32_t BufferedPageMemory::write(uint32_t address, uint8_t const *data, size_t length, MemoryWriteFlags flags) {
    auto g = target_->geometry();
    auto page = address / g.page_size;
    if (cached_ == UINT32_MAX || page != cached_) {
        if (dirty_) {
            auto rv = flush();
            if (rv <= 0) {
                return rv;
            }
        }

        auto rv = target_->read(page * g.page_size, buffer_, g.page_size, MemoryReadFlags::None);
        if (rv <= 0) {
            return rv;
        }

        cached_ = page;
    }

    auto page_offset = address % g.page_size;
    memcpy(buffer_ + page_offset, data, length);
    dirty_ = true;

    if (dirty_start_ == -1) {
        dirty_start_ = page_offset;
    }
    else {
        FK_ASSERT((int16_t)page_offset >= dirty_end_);
    }
    dirty_end_ = page_offset + length;

    return length;
}

int32_t BufferedPageMemory::erase(uint32_t address, size_t length) {
    if (cached_ != UINT32_MAX) {
        auto g = target_->geometry();
        auto cached_address = cached_ * g.page_size;
        if (cached_address >= address && cached_address < address + length) {
            cached_ = UINT32_MAX;
            dirty_start_ = -1;
            dirty_end_ = -1;
            dirty_ = false;
        }
    }
    return target_->erase(address, length);
}

int32_t BufferedPageMemory::flush() {
    if (cached_ != UINT32_MAX && dirty_) {
        auto g = target_->geometry();
        auto address = cached_ * g.page_size;

        logdebug("[" PRADDRESS "] flush dirty page (0x%4x - 0x%4x)", address, dirty_start_, dirty_end_);

        auto rv = target_->write(address, buffer_, g.page_size);
        cached_ = UINT32_MAX;
        dirty_start_ = -1;
        dirty_end_ = -1;
        dirty_ = false;
        if (rv <= 0) {
            return rv;
        }
    }
    return target_->flush();
}

} // namespace fk
