#pragma once

#include <flash_memory.h>

namespace phylum {

class memory_flash_memory : public flash_memory {
private:
    size_t page_size_ { 0 };
    uint8_t *memory_{ nullptr };

public:
    memory_flash_memory(size_t page_size) : page_size_(page_size) {
        memory_ = (uint8_t *)malloc(block_size() * number_blocks());
        memset(memory_, 0xff, block_size() * number_blocks());
    }

    virtual ~memory_flash_memory() {
        free(memory_);
    }

public:
    size_t block_size() override {
        assert(page_size_ > 0);
        return 128 * page_size_;
    }

    size_t number_blocks() override {
        return 256;
    }

    size_t page_size() override {
        return page_size_;
    }

    int32_t erase(uint32_t address, uint32_t length) override {
        // phydebugf("[0x%08" PRIx32 "] flash erase %" PRIu32, address, length);
        memset(memory_ + address, 0xff, length);
        return 0;
    }

    int32_t write(uint32_t address, uint8_t const *data, size_t size) override {
        // phydebugf("[0x%08" PRIx32 "] write %" PRIu32, address, size);
        memcpy(memory_ + address, data, size);
        return 0;
    }

    int32_t read(uint32_t address, uint8_t *data, size_t size) override {
        // phydebugf("[0x%08" PRIx32 "] read %" PRIu32, address, size);
        memcpy(data, memory_ + address, size);
        return 0;
    }

    int32_t copy_page(uint32_t source, uint32_t destiny, size_t size) override {
        // phydebugf("[0x%08" PRIx32 "] copy 0x%08" PRIx32 " %", destiny, source, size);
        memcpy(memory_ + destiny, memory_ + source, size);
        return 0;
    }
};

} // namespace phylum
