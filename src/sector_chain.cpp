#include "sector_chain.h"
#include "directory_chain.h" // For file_entry_t, etc...
#include "data_chain.h"

namespace phylum {

int32_t sector_chain::create_if_necessary() {
    logged_task lt{ "create" };

    if (head_ != InvalidSector || tail_ != InvalidSector) {
        return 0;
    }

    name("creating");

    auto err = grow_tail();
    if (err < 0) {
        return err;
    }

    phydebugf("%s: created, ready", name());

    return 0;
}

int32_t sector_chain::flush() {
    logged_task lt{ "sc-flush" };

    assert_valid();

    if (!dirty()) {
        phydebugf("%s flush (NOOP)", name());
        return 0;
    }

    phydebugf("%s flush", name());

    auto err = sectors_->write(sector_, buffer_.read_view().ptr(), buffer_.read_view().size());
    if (err < 0) {
        return err;
    }

    dirty(false);

    return 0;
}

int32_t sector_chain::seek_end_of_chain() {
    logged_task lt{ "seek-eoc" };

    assert_valid();

    phydebugf("%s starting", name());

    while (true) {
        auto err = forward();
        if (err < 0) {
            phydebugf("%s end (%d)", name(), err);
            return err;
        } else if (err == 0) {
            break;
        }
    }

    auto err = seek_end_of_buffer();
    if (err < 0) {
        return err;
    }

    phydebugf("%s sector=%d position=%zu", name(), sector_, db().position());

    return 0;
}

int32_t sector_chain::back_to_head() {
    if (sector_ != InvalidSector) {
        phydebugf("%s back-to-head %d -> %d", name(), sector_, head_);
        sector_ = InvalidSector;
    } else {
        phydebugf("%s back-to-head %d (NOOP)", name(), sector_);
    }

    length_sectors_ = 0;

    db().rewind();

    return 0;
}

int32_t sector_chain::back_to_tail() {
    assert(false);
    return 0;
}

int32_t sector_chain::forward() {
    logged_task lt{ "forward" };

    assert_valid();

    appendable(false);

    if (sector_ == InvalidSector) {
        phydebugf("%s first load", name());
        sector_ = head_;
    } else {
        auto hdr = header<sector_chain_header_t>();
        if (hdr->np == 0 || hdr->np == UINT32_MAX) {
            if (hdr->type == entry_type::DataSector) {
                auto dchdr = header<data_chain_header_t>();
                phydebugf("%s sector=%d bytes=%d length=%d (end)", name(), sector_, dchdr->bytes, length_sectors_);
            } else {
                phydebugf("%s sector=%d length=%d (end)", name(), sector_, length_sectors_);
            }
            return 0;
        }

        sector(hdr->np);

        if (hdr->type == entry_type::DataSector) {
            auto dchdr = header<data_chain_header_t>();
            phydebugf("%s sector=%d bytes=%d length=%d", name(), sector_, dchdr->bytes, length_sectors_);
        } else {
            phydebugf("%s sector=%d length=%d", name(), sector_, length_sectors_);
        }
    }

    db().clear();

    auto err = load();
    if (err < 0) {
        phydebugf("%s: load failed", name());
        return err;
    }

    length_sectors_++;

    return 1;
}

int32_t sector_chain::load() {
    logged_task lt{ "load" };

    assert_valid();

    if (sector_ == InvalidSector) {
        phydebugf("invalid sector");
        return -1;
    }

    db().rewind();

    return buffer_.unsafe_all([&](uint8_t *ptr, size_t size) {
        return sectors_->read(sector_, ptr, size);
    });
}

int32_t sector_chain::log() {
    logged_task lt{ "log" };

    return walk([&](entry_t const *entry, written_record &record) {
        logged_task lt{ this->name() };

        switch (entry->type) {
        case entry_type::None: {
            phyinfof("none (%zu)", record.size_of_record);
            break;
        }
        case entry_type::SuperBlock: {
            auto sb = record.as<super_block_t>();
            phyinfof("super-block (%zu) version=%d", record.size_of_record, sb->version);
            break;
        }
        case entry_type::DirectorySector: {
            auto sh = record.as<sector_chain_header_t>();
            phyinfof("dir-sector (%zu) p=%d n=%d", record.size_of_record, sh->pp, sh->np);
            break;
        }
        case entry_type::DataSector: {
            auto sh = record.as<data_chain_header_t>();
            phyinfof("data-sector (%zu) p=%d n=%d bytes=%d", record.size_of_record, sh->pp, sh->np, sh->bytes);
            break;
        }
        case entry_type::FileEntry: {
            auto fe = record.as<file_entry_t>();
            phyinfof("entry (%zu) id=0x%x name='%s'", record.size_of_record, fe->id, fe->name);
            break;
        }
        case entry_type::FileData: {
            auto fd = record.as<file_data_t>();
            if (fd->size > 0) {
                phyinfof("data (%zu) id=0x%x size=%d", record.size_of_record, fd->id, fd->size);
            } else {
                phyinfof("data (%zu) id=0x%x chain=%d/%d", record.size_of_record, fd->id, fd->chain.head,
                       fd->chain.tail);

                data_chain dc{ *this, fd->chain };
                phyinfof("chain total-bytes=%d", dc.total_bytes());
            }
            break;
        }
        case entry_type::FileAttribute: {
            auto fa = record.as<file_attribute_t>();
            phyinfof("attr (%zu) id=0x%x attr=%d", record.size_of_record, fa->id, fa->type);
            break;
        }
        case entry_type::TreeNode: {
            auto node = record.as<tree_node_header_t>();
            phyinfof("node (%zu) id=0x%x", record.size_of_record, node->id);
            break;
        }
        }
        return 0;
    });
}

int32_t sector_chain::write_header_if_at_start() {
    if (db().position() > 0) {
        return 0;
    }

    phydebugf("%s write header", name());

    auto err = write_header();
    if (err < 0) {
        return err;
    }

    return 1;
}

int32_t sector_chain::grow_head() {
    assert(false);

    return 0;
}

int32_t sector_chain::grow_tail() {
    logged_task lt{ "grow" };

    auto previous_sector = sector_;
    auto allocated = allocator_->allocate();
    assert(allocated != sector_); // Don't ask.

    phydebugf("%s grow! %zu/%zu alloc=%d", name(), db().position(), db().size(), allocated);

    if (sector_ != InvalidSector) {
        assert(db().begin() != db().end());

        auto hdr = header<sector_chain_header_t>();
        assert(hdr != nullptr);
        hdr->np = allocated;
        dirty(true);

        auto err = flush();
        if (err < 0) {
            return err;
        }
    }

    tail(allocated);
    sector(allocated);

    buffer_.clear();
    length_sectors_++;

    auto err = write_header();
    if (err < 0) {
        return err;
    }

    auto hdr = header<sector_chain_header_t>();
    assert(hdr != nullptr);
    hdr->pp = previous_sector;

    return 0;
}

void sector_chain::name(const char *f, ...) {
    va_list args;
    va_start(args, f);
    phy_vsnprintf(name_, sizeof(name_), f, args);
    va_end(args);
}

} // namespace phylum
