#include <algorithm>

#include "storage/caching_memory.h"

namespace fk {

FK_DECLARE_LOGGER("memory");

void CachedPage::mark_dirty(uint16_t offset, uint16_t length) {
    dirty_start = std::min<uint16_t>(offset, dirty_start);
    dirty_end = std::max<uint16_t>(offset + length, dirty_end);
    ts = fk_uptime() + 1;
    logtrace("[0x%06" PRIx32 "]: marked #%" PRIu32 " dirty (0x%4x - 0x%4x)", page * 2048, page, offset, offset + length);
}

bool CachedPage::dirty() const {
    return dirty_start != UINT16_MAX;
}

CachingMemory::CachingMemory(DataMemory *target, PageCache *cache) : target_(target), cache_(cache) {
}

bool CachingMemory::begin() {
    if (!target_->begin()) {
        return false;
    }

    if (!cache_->invalidate()) {
        return false;
    }

    return true;
}

FlashGeometry CachingMemory::geometry() const {
    return target_->geometry();
}

int32_t CachingMemory::read(uint32_t address, uint8_t *data, size_t length) {
    auto page_size = target_->geometry().page_size;
    auto page = cache_->get_page(address, false);
    if (page == nullptr) {
        logerror("page lookup failed (0x%" PRIx32 ") (for read)", address);
        return 0;
    }
    FK_ASSERT(page->ptr != nullptr);
    memcpy(data, page->ptr + (address % page_size), length);
    return length;
}

#if defined(linux)
static void verify_erased(uint32_t address, uint8_t *p, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        if (*p != 0xff) {
            logerror("corruption: 0x%x", address);
            assert(*p == 0xff);
        }
        p++;
    }
}
#endif

int32_t CachingMemory::write(uint32_t address, const uint8_t *data, size_t length) {
    auto page_size = target_->geometry().page_size;
    auto page = cache_->get_page(address, true);
    if (page == nullptr) {
        logerror("page lookup failed (0x%" PRIx32 ") (for write)", address);
        return 0;
    }
    FK_ASSERT(page->ptr != nullptr);

    auto offset = (address % page_size);
    auto p = page->ptr + offset;
    #if defined(linux)
    verify_erased(address, p, length);
    #endif

    memcpy(p, data, length);
    page->mark_dirty(offset, length);
    return length;
}

int32_t CachingMemory::erase_block(uint32_t address) {
    if (!target_->erase_block(address)) {
        return false;
    }

    FK_ASSERT(cache_->invalidate(address));

    return true;
}

int32_t CachingMemory::flush() {
    if (!target_->flush()) {
        logerror("memory flush failed");
        return false;
    }

    if (!cache_->flush()) {
        logerror("cache flush failed");
        return false;
    }

    return true;
}

}
