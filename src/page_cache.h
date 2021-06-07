#pragma once

extern "C" {

#include <dhara/error.h>
#include <dhara/map.h>

}

#include "phylum.h"
#include "simple_buffer.h"

namespace phylum {

class sector_page_cache {
public:
    virtual bool get(dhara_sector_t sector, dhara_page_t *page) = 0;
    virtual bool set(dhara_sector_t sector, dhara_page_t page) = 0;

};

class noop_page_cache : public sector_page_cache {
public:
    bool get(dhara_sector_t /*sector*/, dhara_page_t */*page*/) override {
        return false;
    }

    bool set(dhara_sector_t /*sector*/, dhara_page_t /*page*/) {
        return true;
    }

    void debug() {
    }
};

class simple_page_cache : public sector_page_cache {
private:
    simple_buffer buffer_;
    struct cache_entry_t {
        dhara_sector_t sector;
        dhara_page_t page;
        uint16_t age;
    };
    cache_entry_t *entries_{ nullptr };
    size_t size_{ 0 };
    uint32_t counter_{ 0 };

public:
    simple_page_cache(simple_buffer buffer);
    simple_page_cache(simple_page_cache &&other);
    virtual ~simple_page_cache();

public:
    bool get(dhara_sector_t sector, dhara_page_t *page) override;
    bool set(dhara_sector_t sector, dhara_page_t page) override;
    void debug();

private:
    bool better_drop_candidate(cache_entry_t const &candidate, cache_entry_t const &selected);

};

} // namespace phylum
