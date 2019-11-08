#pragma once

#include "common.h"
#include "hal/memory.h"

namespace fk {

struct CachedPage {
    uint32_t ts{ 0 };
    uint32_t page{ 0 };
    uint8_t *ptr{ nullptr };
    uint16_t dirty_start{ UINT16_MAX };
    uint16_t dirty_end{ 0 };

    void mark_dirty(uint16_t offset, uint16_t length);
    void mark_clean() {
        dirty_start = UINT16_MAX;
        dirty_end = 0;
    }
    bool dirty() const;
};

class PageCache {
public:
    virtual CachedPage *get_page(uint32_t address, bool writing) = 0;
    virtual size_t invalidate(uint32_t address) = 0;
    virtual size_t invalidate() = 0;
    virtual bool flush(CachedPage *page) = 0;
    virtual bool flush() = 0;
};

class CachingMemory : public DataMemory {
private:
    DataMemory *target_;
    PageCache *cache_;

public:
    CachingMemory(DataMemory *target, PageCache *cache);

public:
    bool begin() override;

    flash_geometry_t geometry() const override;

    int32_t read(uint32_t address, uint8_t *data, size_t length) override;

    int32_t write(uint32_t address, const uint8_t *data, size_t length) override;

    int32_t erase_block(uint32_t address) override;

    int32_t flush() override;

};

}
