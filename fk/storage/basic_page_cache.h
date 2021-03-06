#pragma once

#include "storage/caching_memory.h"

namespace fk {

template<typename PageStoreType, size_t PageSize, size_t N>
class BasicPageCache : public PageCache {
private:
    FK_DECLARE_LOGGER_MEMBER("pagecache");

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

private:
    CachedPage *get_tail_dirty_page() {
        CachedPage *selected = nullptr;

        for (auto &p : pages_) {
            if (p.dirty()) {
                if (selected == nullptr || p.page > selected->page) {
                    selected = &p;
                }
            }
        }

        return selected;
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

    void invalidate(CachedPage *page) {
        page->mark_clean();
        page->ts = 0;
        page->page = 0;
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
                invalidate(p);
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
            invalidate(p);
        }

        return true;
    }

    bool flush(CachedPage *page) override {
        if (!page->dirty()) {
            return true;
        }

        auto page_address = (uint32_t)page->page * PageSize;

        logdebug("[0x%06" PRIx32 "]: flush #%" PRIu32 " dirty (0x%3x - 0x%3x) (0x%06" PRIx32 " - 0x%06" PRIx32 ") %" PRIu32 " bytes",
                 page_address, (uint32_t)page->page, page->dirty_start, page->dirty_end,
                 page_address + page->dirty_start, page_address + page->dirty_end,
                 page->dirty_end - page->dirty_start);

        if (!store_.save_page(page_address, page->ptr, PageSize, page->dirty_start, page->dirty_end)) {
            logerror("[0x%06" PRIx32 "]: flush #%" PRIu32 " dirty (0x%3x - 0x%3x) (0x%06" PRIx32 " - 0x%06" PRIx32 ") %" PRIu32 " bytes failed!",
                     page_address, (uint32_t)page->page, page->dirty_start, page->dirty_end,
                     page_address + page->dirty_start, page_address + page->dirty_end,
                     page->dirty_end - page->dirty_start);
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

    int32_t number_of_dirty_blocks() const {
        int32_t blocks[N];
        for (auto i = 0u; i < N; ++i) {
            blocks[i] = -1;
        }
        for (auto i = 0u; i < N; ++i) {
            if (pages_[i].dirty()) {
                auto block = pages_[i].block();
                for (auto j = 0u; j < N; ++j) {
                    if (blocks[j] == block) {
                        break;
                    }
                    if (blocks[j] == -1) {
                        blocks[j] = block;
                        break;
                    }
                }
            }
        }
        for (auto i = 0u; i < N; ++i) {
            if (blocks[i] == -1) {
                return i;
            }
        }
        return N;
    }

    bool flush() override {
        auto nblocks = number_of_dirty_blocks();
        auto ndirty = number_of_dirty_pages();
        if (ndirty > 1) {
            logdebug("flushing %d dirty pages (%d blocks)", ndirty, nblocks);
        }

        FK_ASSERT(nblocks <= 2);

        if (nblocks == 2) {
        }

        auto success = true;
        for (auto i = 0u; i < N; ++i) {
            auto flushing = get_tail_dirty_page();
            if (flushing == nullptr) {
                break;
            }
            if (!flush(flushing)) {
                success = false;
                invalidate(flushing);
            }
        }

        return success;
    }

};

}
