#include <algorithm>

#include "storage/sequential_memory.h"

namespace fk {

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
        if (!target_->read(address, p, reading)) {
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
        if (!target_->write(address, p, writing)) {
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

CacheSinglePageMemory::CacheSinglePageMemory(DataMemory *target) : target_(target) {
    buffer_ = (uint8_t *)fk_malloc(DefaultWorkerPoolSize);
}

CacheSinglePageMemory::~CacheSinglePageMemory() {
    if (buffer_ != nullptr) {
        printf("%x FREE\n", buffer_);
        fk_free(buffer_);
        buffer_ = nullptr;
    }
}

bool CacheSinglePageMemory::begin() {
    return target_->begin();
}

FlashGeometry CacheSinglePageMemory::geometry() const {
    return target_->geometry();
}

int32_t CacheSinglePageMemory::read(uint32_t address, uint8_t *data, size_t length, MemoryReadFlags flags) {
    auto g = target_->geometry();
    auto page = address / g.page_size;
    if (cached_ == UINT32_MAX || page != cached_) {
        auto rv = target_->read(page * g.page_size, buffer_, g.page_size, flags);
        if (rv <= 0) {
            return rv;
        }
    }
    auto page_offset = address % g.page_size;
    memcpy(data, buffer_ + page_offset, length);
    return length;
}

int32_t CacheSinglePageMemory::write(uint32_t address, uint8_t const *data, size_t length, MemoryWriteFlags flags) {
    return target_->write(address, data, length, flags);
}

int32_t CacheSinglePageMemory::erase_block(uint32_t address) {
    return target_->erase_block(address);
}

int32_t CacheSinglePageMemory::flush() {
    return target_->flush();
}

} // namespace fk
