#include "sector_chain.h"
#include "data_chain.h"
#include "directory_chain.h" // For file_entry_t, etc...

namespace phylum {

int32_t sector_chain::create_if_necessary() {
    assert(head_ == InvalidSector && tail_ == InvalidSector);

    phyverbosef("creating sector=%d", sector());

    auto page_lock = db().writing(sector());

    auto err = grow_tail(page_lock);
    if (err < 0) {
        return err;
    }

    err = flush(page_lock);
    if (err < 0) {
        return err;
    }

    phyverbosef("%s: created, ready", name());

    return 0;
}

int32_t sector_chain::flush() {
    auto page_lock = db().writing(sector());

    page_lock.dirty();

    return flush(page_lock);
}

int32_t sector_chain::flush(page_lock &page_lock) {
    assert_valid();

    phyverbosef("%s flush", name());

    auto err = page_lock.flush(sector_);
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t sector_chain::seek_end_of_chain(page_lock &page_lock) {
    assert_valid();

    phyverbosef("%s starting", name());

    while (true) {
        auto err = forward(page_lock);
        if (err < 0) {
            phyverbosef("%s end (%d)", name(), err);
            return err;
        } else if (err == 0) {
            break;
        }
    }

    auto err = seek_end_of_buffer(page_lock);
    if (err < 0) {
        return err;
    }

    phyverbosef("%s sector=%d position=%zu", name(), sector_, db().position());

    return 0;
}

int32_t sector_chain::back_to_head(page_lock &page_lock) {
    phyverbosef("%s back-to-head %d -> %d", name(), sector_, head_);

    auto err = page_lock.replace(head_);
    if (err < 0) {
        return 0;
    }

    db().rewind();

    sector_ = head_;
    visited_sectors_ = 0;

    return 0;
}

int32_t sector_chain::forward(page_lock &page_lock) {
    assert_valid();

    appendable(false);

    if (sector_ == InvalidSector) {
        phyverbosef("%s first load", name());
        sector_ = head_;
    } else {
        auto hdr = db().header<sector_chain_header_t>();
        if (((int32_t)hdr->flags & (int32_t)sector_flags::Tail) > 0) {
            if (hdr->type == entry_type::DataSector) {
                auto dchdr = db().header<data_chain_header_t>();
                phyverbosef("%s sector=%d bytes=%d visited=%d (tail)", name(), sector_, dchdr->bytes, visited_sectors_);
            } else {
                phyverbosef("%s sector=%d visited=%d (tail)", name(), sector_, visited_sectors_);
            }
            return 0;
        }
        if (hdr->np == 0 || hdr->np == UINT32_MAX) {
            if (hdr->type == entry_type::DataSector) {
                auto dchdr = db().header<data_chain_header_t>();
                phyverbosef("%s sector=%d bytes=%d visited=%d (end)", name(), sector_, dchdr->bytes, visited_sectors_);
            } else {
                phyverbosef("%s sector=%d visited=%d (end)", name(), sector_, visited_sectors_);
            }
            return 0;
        }

        sector(hdr->np);

        if (hdr->type == entry_type::DataSector) {
            auto dchdr = db().header<data_chain_header_t>();
            phyverbosef("%s sector=%d bytes=%d visited=%d", name(), sector_, dchdr->bytes, visited_sectors_);
        } else {
            phyverbosef("%s sector=%d visited=%d", name(), sector_, visited_sectors_);
        }
    }

    auto err = load(page_lock);
    if (err < 0) {
        phyerrorf("%s: load failed", name());
        return err;
    }

    visited_sectors_++;

    return 1;
}

int32_t sector_chain::prepare_sector(page_lock &lock, dhara_sector_t previous_sector, bool preserve_header) {
    dhara_sector_t following_sector = InvalidSector;

    if (preserve_header) {
        auto hdr = db().header<sector_chain_header_t>();
        following_sector = hdr->np;
    }

    db().clear();

    auto err = write_header(lock);
    if (err < 0) {
        return err;
    }

    phyverbosef("prepare-sector pp=%d np=%d", previous_sector, following_sector);

    assert(db().write_header<sector_chain_header_t>([&](sector_chain_header_t *header) {
        header->pp = previous_sector;
        header->np = following_sector;
        header->flags = sector_flags::Tail;
        return 0;
    }) == 0);

    return 0;
}

int32_t sector_chain::truncate() {
    assert(head_ != InvalidSector && tail_ != InvalidSector);

    sector(head_);

    auto lock = db().writing(sector());

    auto err = prepare_sector(lock, InvalidSector, true);
    if (err < 0) {
        return err;
    }

    err = flush(lock);
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t sector_chain::load(page_lock &page_lock) {
    // logged_task lt{ "load" };

    assert_valid();

    if (sector_ == InvalidSector) {
        phyerrorf("invalid sector");
        return -1;
    }

    auto err = page_lock.replace(sector_);
    if (err < 0) {
        return err;
    }

    db().rewind();

    return 0;
}

int32_t sector_chain::log(bool graph) {
    logged_task lt{ "log" };

    assert_valid();

    if (graph) {
        phygraphf("digraph g {");

        phygraphf("graph [ rankdir = \"LR\" ];");
        phygraphf("node [ fontsize = \"16\" ];");
    }

    auto load_fn = [&](dhara_sector_t previous, dhara_sector_t sector) -> int32_t {
        if (graph) {
            phygraphf("\"sector_%d\" [", sector);
            phygraphf("  label = \"<f0> %d\"", sector);
            phygraphf("  shape = \"record\"");
            phygraphf("]");
            if (previous != InvalidSector) {
                phygraphf("\"sector_%d\":f0 -> \"sector_%d\":f0 []", previous, sector);
            }
        }
        return 0;
    };

    auto walk_fn = [&](page_lock &/*page_lock*/, entry_t const *entry, record_ptr &record) -> int32_t {
        // logged_task lt{ this->name() };

        switch (entry->type) {
        case entry_type::None: {
            phyinfof("none (%zu)", record.size_of_record());
            break;
        }
        case entry_type::SuperBlock: {
            auto sb = record.as<super_block_t>();
            phyinfof("super-block (%zu) version=%d", record.size_of_record(), sb->version);
            break;
        }
        case entry_type::DirectorySector: {
            auto sh = record.as<sector_chain_header_t>();
            phyinfof("dir-sector (%zu) p=%d n=%d", record.size_of_record(), sh->pp, sh->np);
            break;
        }
        case entry_type::TreeSector: {
            auto sh = record.as<sector_chain_header_t>();
            phyinfof("tree-sector (%zu) p=%d n=%d", record.size_of_record(), sh->pp, sh->np);
            break;
        }
        case entry_type::DataSector: {
            auto sh = record.as<data_chain_header_t>();
            phyinfof("data-chain-sector (%zu) p=%d n=%d bytes=%d", record.size_of_record(), sh->pp, sh->np, sh->bytes);
            break;
        }
        case entry_type::FreeChainSector: {
            auto sh = record.as<free_chain_header_t>();
            phyinfof("free-chain-sector (%zu) p=%d n=%d", record.size_of_record(), sh->pp, sh->np);
            break;
        }
        case entry_type::FileEntry: {
            auto fe = record.as<file_entry_t>();
            phyinfof("entry (%zu) id=0x%x name='%s'", record.size_of_record(), fe->id, fe->name);
            break;
        }
        case entry_type::FsDirectoryEntry: {
            auto fe = record.as<dirtree_dir_t>();
            phyinfof("entry (%zu) dir name='%s'", record.size_of_record(), fe->name);
            break;
        }
        case entry_type::FsFileEntry: {
            auto fe = record.as<dirtree_file_t>();
            phyinfof("entry (%zu) file name='%s'", record.size_of_record(), fe->name);
            break;
        }
        case entry_type::FileData: {
            auto fd = record.as<file_data_t>();
            if (fd->size > 0) {
                phyinfof("data (%zu) id=0x%x size=%d", record.size_of_record(), fd->id, fd->size);
            } else {
                phyinfof("data (%zu) id=0x%x chain=%d/%d", record.size_of_record(), fd->id, fd->chain.head,
                         fd->chain.tail);
            }
            break;
        }
        case entry_type::FileAttribute: {
            auto fa = record.as<file_attribute_t>();
            phyinfof("attr (%zu) id=0x%x attr=%d", record.size_of_record(), fa->id, fa->type);
            break;
        }
        case entry_type::TreeNode: {
            auto node = record.as<tree_node_header_t>();
            phyinfof("node (%zu) depth=%d nkeys=0x%x", record.size_of_record(), node->depth, node->number_keys);
            break;
        }
        case entry_type::FreeSectors: {
            auto node = record.as<free_sectors_t>();
            phyinfof("free-sectors (%zu) head=%d", record.size_of_record(), node->head);
            break;
        }
        }
        return 0;
    };

    auto err = walk(walk_fn, load_fn);

    if (graph) {
        phygraphf("}");
    }

    return err;
}

int32_t sector_chain::write_header_if_at_start(page_lock &page_lock) {
    if (db().position() > 0) {
        return 0;
    }

    phyverbosef("%s write header", name());

    auto err = write_header(page_lock);
    if (err < 0) {
        return err;
    }

    return 1;
}

int32_t sector_chain::grow_tail(page_lock &lock) {
    // logged_task lt{ "grow" };

    auto previous_sector = sector_;
    auto following_sector = InvalidSector;
    auto allocated = true;

    if (sector_ != InvalidSector) {
        assert(db().begin() != db().end());

        assert(db().write_header<sector_chain_header_t>([&](sector_chain_header_t *header) {
            if (header->np == InvalidSector) {
                header->np = allocator_->allocate();
            }
            else {
                allocated = false;
            }
            header->flags = sector_flags::None;
            following_sector = header->np;
            return 0;
        }) == 0);

        lock.dirty();

        auto err = flush(lock);
        if (err < 0) {
            return err;
        }
    }
    else {
        following_sector = allocator_->allocate();
    }

    tail(following_sector);
    sector(following_sector);

    auto err = lock.replace(following_sector, allocated);
    if (err < 0) {
        return err;
    }

    err = prepare_sector(lock, previous_sector, !allocated);
    if (err < 0) {
        return err;
    }

    visited_sectors_++;

    lock.dirty();

    return 0;
}

void sector_chain::name(const char *f, ...) {
    va_list args;
    va_start(args, f);
    phy_vsnprintf(name_, sizeof(name_), f, args);
    va_end(args);
}

int32_t sector_chain::dequeue_sector(dhara_sector_t *sector) {
    assert(sector != nullptr);
    *sector = InvalidSector;

    if (head_ == InvalidSector) {
        return -1;
    }

    auto page_lock = db().reading(head());

    auto hdr = db().header<sector_chain_header_t>();
    *sector = head_;
    head_ = hdr->np;

    phyverbosef("dequeue sector=%d head=%d", *sector, head_);

    return 1;
}

} // namespace phylum
