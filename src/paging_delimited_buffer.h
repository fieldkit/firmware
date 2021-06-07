#pragma once

#include "delimited_buffer.h"
#include "sector_allocator.h"
#include "working_buffers.h"

namespace phylum {

class paging_delimited_buffer;

class page_lock {
private:
    paging_delimited_buffer *buffer_{ nullptr };
    dhara_sector_t sector_{ InvalidSector };
    bool read_only_{ true };
    bool dirty_{ false };

public:
    page_lock(page_lock &other) = delete;

    page_lock(page_lock &&other);

    page_lock(paging_delimited_buffer *buffer, dhara_sector_t sector, bool read_only, bool overwrite);

    virtual ~page_lock();

public:
    int32_t replace(dhara_sector_t sector, bool overwrite = false);

    int32_t flush(dhara_sector_t sector);

    int32_t dirty(bool dirty = true);

    bool is_dirty() const {
        return dirty_;
    }

    dhara_sector_t sector() const {
        return sector_;
    }

    paging_delimited_buffer &db() {
        return *buffer_;
    }

};

class paging_delimited_buffer : public delimited_buffer {
private:
    working_buffers *buffers_{ nullptr };
    sector_map *sectors_{ nullptr };
    dhara_sector_t sector_{ InvalidSector };
    bool valid_{ false };

public:
    paging_delimited_buffer(working_buffers &buffers, sector_map &sectors);

public:
    page_lock reading(dhara_sector_t sector);

    page_lock writing(dhara_sector_t sector);

    page_lock overwrite(dhara_sector_t sector);

protected:
    void ensure_valid() const override;

    int32_t replace(dhara_sector_t sector, bool read_only, bool overwrite = false);

    int32_t release(dhara_sector_t sector);

    int32_t flush(dhara_sector_t sector);

    friend class page_lock;
};

} // namespace phylum
