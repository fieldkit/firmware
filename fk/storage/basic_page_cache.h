#pragma once

#include "storage/caching_memory.h"

namespace fk {

template<typename PageStoreType, size_t PageSize, size_t N>
class BasicPageCache : public PageCache {
private:
    void *memory_;
    PageStoreType store_;
    CachedPage pages_[N];

public:
    BasicPageCache(PageStoreType store) : store_(store) {
        memory_ = fk_malloc(N * PageSize);
        for (size_t i = 0; i < N; ++i) {
            pages_[i] = { };
            pages_[i].ptr = ((uint8_t *)memory_) + i * PageSize;
        }
    }

public:
    CachedPage *get_page(uint32_t address, bool writing) override {
        CachedPage *available = nullptr;
        CachedPage *old = nullptr;

        uint32_t page = address / PageSize;

        for (size_t i = 0; i < N; ++i) {
            auto p = &pages_[i];
            if (false) logverbose("page[%zd] page=#%" PRIu32 " ts=%" PRIu32 " %s", i, p->page, p->ts, p->dirty() ? "dirty": "");
            if (p->ts == 0) {
                available = p;
                old = p;
            } else {
                if (p->page == page) {
                    if (false) logverbose("existing page #%" PRIu32 " (%" PRIu32 ")", p->page, p->ts);
                    return p;
                }
                if (old == nullptr || old->ts > p->ts) {
                    if (!p->dirty()) {
                        old = p;
                    }
                }
            }
        }

        FK_ASSERT(old != nullptr);

        if (available == nullptr) {
            FK_ASSERT(!old->dirty());
            if (!flush(old)) {
                logerror("page #%" PRIu32 " flush failed (0x%06" PRIx32 ")", old->page, old->page * PageSize);
                return nullptr;
            }
            available = old;
        }

        FK_ASSERT(available != nullptr);

        available->ts = fk_uptime() + 1;
        available->page = page;
        available->mark_clean();

        if (false) logtrace("load page #%" PRIu32 " (%" PRIu32 ")", available->page, available->ts);

        if (!store_.load_page(page * PageSize, available->ptr, PageSize)) {
            logerror("page #%" PRIu32 " load failed", page);
            return nullptr;
        }

        return available;
    }

    size_t invalidate(uint32_t address) override {
        uint32_t page = address / PageSize;
        for (size_t i = 0; i < N; ++i) {
            auto p = &pages_[i];
            if (p->page == page) {
                if (p->dirty()) {
                    logerror("invalidate DIRTY (%" PRIu32 ")", page);
                    FK_ASSERT(!p->dirty());
                }
                else if (p->ts > 0) {
                    if (false) logtrace("invalidate (%" PRIu32 ")", page);
                }
                p->mark_clean();
                p->ts = 0;
                p->page = 0;
            }
        }

        return true;
    }

    size_t invalidate() override {
        for (size_t i = 0; i < N; ++i) {
            auto p = &pages_[i];
            if (p->dirty()) {
                logerror("invalidate(all) DIRTY (%" PRIu32 ")", p->page);
                FK_ASSERT(!p->dirty());
            }
            else if (p->ts > 0) {
                if (false) logtrace("invalidate(all) (%" PRIu32 ")", p->page);
            }
            p->mark_clean();
            p->ts = 0;
            p->page = 0;
        }

        return true;
    }

    bool flush(CachedPage *page) override {
        if (!page->dirty()) {
            return true;
        }

        logdebug("[0x%06" PRIx32 "]: flush #%" PRIu32 " dirty (0x%x - 0x%x)", (uint32_t)(page->page * PageSize), (uint32_t)page->page, page->dirty_start, page->dirty_end);

        if (!store_.save_page(page->page * PageSize, page->ptr, PageSize, page->dirty_start, page->dirty_end)) {
            logerror("[0x%06" PRIx32 "]: flush #%" PRIu32 " dirty (0x%x - 0x%x) failed!", (uint32_t)(page->page * PageSize), (uint32_t)page->page, page->dirty_start, page->dirty_end);
            return false;
        }

        page->mark_clean();

        return true;
    }

    int32_t number_of_dirty_pages() const {
        auto n = 0u;
        for (auto i = 0u; i < N; ++i) {
            if (pages_[i].dirty()) {
                n++;
            }
        }
        return n;
    }

    bool flush() override {
        auto ndirty = number_of_dirty_pages();
        if (ndirty > 0) {
            logdebug("flushing %d dirty pages", ndirty);
        }

        auto success = true;

        for (auto i = 0u; i < N; ++i) {
            if (!flush(&pages_[i])) {
                success = false;
            }
        }

        return success;
    }

private:
    FK_DECLARE_LOGGER_MEMBERS("pagecache");

};

}
