#include "paging_delimited_buffer.h"

namespace phylum {

page_lock::page_lock(paging_delimited_buffer *buffer, dhara_sector_t sector, bool read_only, bool overwrite)
    : buffer_(buffer), sector_(sector), read_only_(read_only) {
    // TODO This is awkward I think. This is here so that we can
    // create page_lock's with an InvalidSector that get replaced
    // later. I'd much rather find an elegant way to remove this but
    // it's here because of how directory_chain works, in that we
    // create the page_lock before eventually calling prepare which
    // handles initializing the sector. Also makes handling this error
    // more difficult.
    if (sector != InvalidSector) {
        assert(buffer_->replace(sector, read_only_, overwrite) >= 0);
    }
}

page_lock::~page_lock() {
    assert(buffer_->release() == 0);
}

int32_t page_lock::replace(dhara_sector_t sector, bool overwrite) {
    assert(sector != InvalidSector);
    if (sector_ == sector) {
        phydebugf("page-lock: noop replace sector=%d", sector);
        return 0;
    }

    auto was_dirty = is_dirty();

    auto err = buffer_->replace(sector, read_only_, overwrite);
    if (err < 0) {
        return err;
    }

    assert(!was_dirty);

    sector_ = sector;

    return 0;
}

int32_t page_lock::flush(dhara_sector_t sector) {
    assert(sector_ == sector);
    if (is_dirty()) {
        auto err = buffer_->flush(sector);
        if (err < 0) {
            return err;
        }
        dirty(false);
    }
    return 0;
}

int32_t page_lock::dirty(bool dirty) {
    assert(!read_only_);
    if (dirty) {
        assert(buffer_->buffers_->dirty_sector(sector_) >= 0);
    }
    dirty_ = dirty;
    return 0;
}

paging_delimited_buffer::paging_delimited_buffer(working_buffers &buffers, sector_map &sectors) : buffers_(&buffers), sectors_(&sectors) {
}

page_lock paging_delimited_buffer::reading(dhara_sector_t sector) {
    phydebugf("page-lock: opening (rd) %d", sector);
    assert(!valid_);
    return page_lock{ this, sector, true, false };
}

page_lock paging_delimited_buffer::writing(dhara_sector_t sector) {
    phydebugf("page-lock: opening (wr) %d", sector);
    assert(!valid_);
    return page_lock{ this, sector, false, false };
}

page_lock paging_delimited_buffer::overwrite(dhara_sector_t sector) {
    phydebugf("page-lock: overwriting %d", sector);
    assert(!valid_);
    return page_lock{ this, sector, false, true };
}

void paging_delimited_buffer::ensure_valid() const {
    assert(valid_);
}

int32_t paging_delimited_buffer::replace(dhara_sector_t sector, bool read_only, bool overwrite) {
    assert(sector != InvalidSector);

    phydebugf("page-lock: replacing previous=%d sector=%d read-only=%d", sector_, sector, read_only);

    if (sector == sector_) {
        phydebugf("page-lock: replace (noop)");
        return 0;
    }

    auto miss_fn = [=](dhara_sector_t page_sector, uint8_t *buffer, size_t size) -> int32_t {
        phydebugf("wbuffers: miss %d", page_sector);
        assert(size > 0);
        if (overwrite) {
            memset(buffer, 0xff, size);
            return 0;
        }
        return sectors_->read(page_sector, buffer, size);
    };

    auto flush_fn = [=](dhara_sector_t page_sector, uint8_t const *buffer, size_t size) {
        phydebugf("wbuffers: flush %d", page_sector);
        assert(size > 0);
        return sectors_->write(page_sector, buffer, size);
    };

    auto opened = buffers_->open_sector(sector, read_only, miss_fn, flush_fn);

    if (ptr() != nullptr) {
        phydebugf("page-lock: freeing previous");
        buffers_->free(ptr());
        ptr(nullptr, 0);
    }

    ptr(opened, buffers_->buffer_size());

    phydebugf("page-lock: replaced previous=%d sector=%d buffer=0x%x read-only=%d", sector_, sector, opened, read_only);

    sector_ = sector;
    valid_ = true;

    return 0;
}

int32_t paging_delimited_buffer::flush(dhara_sector_t sector) {
    phydebugf("flush have=%d expected=%d", sector_, sector);

    assert(sector_ != InvalidSector);
    assert(sector_ == sector);

    auto flush_fn = [=](dhara_sector_t page_sector, uint8_t const *buffer, size_t size) {
        phydebugf("wbuffers: flush %d", page_sector);
        assert(size > 0);
        return sectors_->write(page_sector, buffer, size);
    };

    auto err = buffers_->flush_sector(sector, flush_fn);
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t paging_delimited_buffer::release() {
    assert(valid_);
    assert(sector_ != InvalidSector);

    valid_ = false;
    sector_ = InvalidSector;
    if (ptr() != nullptr) {
        buffers_->free(ptr());
        ptr(nullptr, 0);
    }

    return 0;
}

} // namespace phylum
