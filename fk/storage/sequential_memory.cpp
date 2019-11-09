#include <algorithm>

#include "storage/sequential_memory.h"

namespace fk {

FK_DECLARE_LOGGER("memory");

template<class T>
static unique_ptr_freed<T> freed_unique_ptr(size_t size) {
	return unique_ptr_freed<T>(static_cast<T*>(malloc(size)), &free);
}

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

BufferedPageMemory::BufferedPageMemory(DataMemory *target)
    : target_(target), buffer_{ freed_unique_ptr<uint8_t>(DefaultWorkerPoolSize) } {
}

BufferedPageMemory::BufferedPageMemory(BufferedPageMemory &&o) : target_(o.target_), buffer_{ std::move(o.buffer_) } {
}

BufferedPageMemory::~BufferedPageMemory() {
}

BufferedPageMemory &BufferedPageMemory::operator=(BufferedPageMemory &&o) {
    target_ = o.target_;
    buffer_ = std::move(o.buffer_);
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
        auto rv = target_->read(page * g.page_size, buffer_.get(), g.page_size, flags);
        if (rv <= 0) {
            return rv;
        }
        cached_ = page;
    }
    auto page_offset = address % g.page_size;
    memcpy(data, buffer_.get() + page_offset, length);
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

        auto rv = target_->read(page * g.page_size, buffer_.get(), g.page_size, MemoryReadFlags::None);
        if (rv <= 0) {
            return rv;
        }
        cached_ = page;
    }
    auto page_offset = address % g.page_size;
    memcpy(buffer_.get() + page_offset, data, length);
    dirty_ = true;
    return length;
}

int32_t BufferedPageMemory::erase_block(uint32_t address) {
    return target_->erase_block(address);
}

int32_t BufferedPageMemory::flush() {
    if (cached_ != UINT32_MAX && dirty_) {
        auto g = target_->geometry();
        logdebug("flush");
        auto rv = target_->write(cached_ * g.page_size, buffer_.get(), g.page_size);
        if (rv <= 0) {
            return rv;
        }
        cached_ = UINT32_MAX;
        dirty_ = false;
    }
    return target_->flush();
}

} // namespace fk
