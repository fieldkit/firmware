#include "phylum.h"
#include "page_cache.h"

namespace phylum {

simple_page_cache::simple_page_cache(simple_buffer buffer) : buffer_(std::move(buffer)) {
    size_ = buffer_.size() / sizeof(cache_entry_t);
    entries_ = (cache_entry_t *)buffer_.ptr();
    buffer_.clear(0xff);
    phyverbosef("page-cache-ready size=%d", size_);
}

simple_page_cache::simple_page_cache(simple_page_cache &&other) : buffer_(std::move(other.buffer_)){
}

simple_page_cache::~simple_page_cache() {
}

bool simple_page_cache::get(dhara_sector_t sector, dhara_page_t *page) {
    for (auto i = 0u; i < size_; ++i) {
        auto &e = entries_[i];
        if (e.sector == sector) {
            phyverbosef("page-cache-got sector=%d page=%d age=%d", sector, e.page, e.age);
            *page = e.page;
            return true;
        }
    }

    return false;
}

bool simple_page_cache::better_drop_candidate(cache_entry_t const &candidate, cache_entry_t const &selected) {
    // Favor entries that don't have a sector in them over those that do.
    if (candidate.sector == InvalidSector && selected.sector != InvalidSector) {
        return true;
    }

    // Favor entries that were used further ago than the selected one.
    if (candidate.age < selected.age) {
        return true;
    }

    return false;
}

bool simple_page_cache::set(dhara_sector_t sector, dhara_page_t page) {
    auto selected = -1;

    for (auto i = 0u; i < size_; ++i) {
        auto &candidate = entries_[i];
        if (candidate.sector == sector) {
            selected = i;
            break;
        }
        else {
            if (selected == -1) {
                selected = i;
            }
            else {
                if (better_drop_candidate(candidate, entries_[selected])) {
                    selected = i;
                }
            }
        }
    }

    assert(selected >= 0);

    auto &e = entries_[selected];
    e.sector = sector;
    e.page = page;
    e.age = ++counter_;

    phyverbosef("page-cache-set sector=%d page=%d age=%d", sector, e.page, e.age);

    return true;
}

void simple_page_cache::debug() {
    phyinfof("page-cache size=%zu", size_);
    for (auto i = 0u; i < size_; ++i) {
        auto &e = entries_[i];
        if (e.sector != InvalidSector) {
            phyinfof("page-cache[%4d] sector=%d page=%d age=%d", i, e.sector, e.page, e.age);
        }
    }
}

} // namespace phylum
