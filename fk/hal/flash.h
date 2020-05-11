#pragma once

#include "common.h"

namespace fk {

class Flash {
private:
    uint32_t page_size_;
    uint32_t total_pages_;

public:
    Flash() { }

public:
    bool initialize();
    int32_t read(uint32_t address, uint8_t *data, size_t size);
    int32_t write(uint32_t address, uint8_t *data, size_t size);
    int32_t erase(uint32_t address, size_t pages);

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
