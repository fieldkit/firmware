#pragma once

#include "common.h"

namespace fk {

class FlashMemory {
public:
    virtual int32_t read(uint32_t address, uint8_t *data, size_t size) = 0;
    virtual int32_t write(uint32_t address, uint8_t const *data, size_t size) = 0;
    virtual int32_t erase(uint32_t address, size_t size) = 0;
    virtual const char *name() = 0;
    virtual uint32_t flash_to_cpu(uint32_t address) {
        return address;
    }
};

class Flash : public FlashMemory {
private:
    uint32_t page_size_{ 0 };
    uint32_t total_pages_{ 0 };

public:
    Flash() { }

public:
    bool initialize();

public:
    int32_t read(uint32_t address, uint8_t *data, size_t size) override;
    int32_t write(uint32_t address, uint8_t const *data, size_t size) override;
    int32_t erase(uint32_t address, size_t size) override;
    const char *name() override {
        return "samd51";
    }

public:
    uint32_t page_size() const {
        return page_size_;
    }

    uint32_t total_pages() const {
        return total_pages_;
    }
};

Flash *get_flash();

}
