#pragma once

#include <map>

#include "sector_map.h"
#include "phylum.h"

namespace phylum {

class memory_sector_map : public sector_map {
private:
    size_t sector_size_{ 256 };
    std::map<dhara_sector_t, uint8_t *> map_;

public:
    memory_sector_map(size_t sector_size) : sector_size_(sector_size) {
    }

    virtual ~memory_sector_map() {
        clear();
    }

public:
    int32_t begin(bool /*force_create*/) override {
        return 0;
    }

    size_t sector_size() override {
        return sector_size_;
    }

    dhara_sector_t size() override {
        return map_.size();
    }

    int32_t trim(dhara_sector_t sector) override {
        if (map_[sector] != nullptr) {
            free(map_[sector]);
            map_[sector] = nullptr;
        }
        return 0;
    }

    int32_t find(dhara_sector_t sector, dhara_page_t *page) override {
        if (map_[sector] == nullptr) {
            return 0;
        }
        *page = 0;
        return -1;
    }

    int32_t write(dhara_sector_t sector, uint8_t const *data, size_t size) override {
        assert(sector != UINT32_MAX);
        assert(size <= sector_size_);
        if (map_[sector] != nullptr) {
            free(map_[sector]);
        }
        map_[sector] = (uint8_t *)malloc(sector_size_);
        memcpy(map_[sector], data, size);
        phydebugf("dhara: write #%d", sector);
        return 0;
    }

    int32_t read(dhara_sector_t sector, uint8_t *data, size_t size) override {
        assert(sector != UINT32_MAX);
        assert(size <= sector_size_);
        phydebugf("dhara: read #%d", sector);
        if (map_[sector] != nullptr) {
            memcpy(data, map_[sector], size);
            return 0;
        }
        return -1;
    }

    int32_t clear() override {
        for (auto const &e : map_) {
            free(e.second);
        }
        map_.clear();
        return 0;
    }

    int32_t sync() override {
        return 0;
    }
};

}
