#include "free_sectors_chain.h"

namespace phylum {

free_sectors_chain::free_sectors_chain(working_buffers &buffers, sector_map &sectors, sector_allocator &allocator,
                                       head_tail_t chain, const char *prefix)
    : record_chain(buffers, sectors, allocator, chain, prefix) {
}

free_sectors_chain::free_sectors_chain(sector_chain &other, head_tail_t chain, const char *prefix)
    : record_chain(other, chain, prefix) {
}

free_sectors_chain::~free_sectors_chain() {
}

int32_t free_sectors_chain::add_free_sectors(free_sectors_t record) {
    assert(record.head != InvalidSector);

    logged_task lt{ "fc-add-chain" };

    auto page_lock = db().writing(sector());

    assert(back_to_head(page_lock) >= 0);

    while (true) {
        for (auto iter = db().begin(); iter != db().end(); ++iter) {
            auto entry = iter->as<entry_t>();
            assert(entry != nullptr);

            if (entry->type == entry_type::FreeSectors) {
                auto fs = iter->as<free_sectors_t>();

                if (fs->head == InvalidSector) {
                    phydebugf("add-chain: reusing: %d", fs->head);

                    auto mutable_record = db().as_mutable<free_sectors_t>(*iter);

                    *mutable_record = record;

                    page_lock.dirty();

                    auto err = page_lock.flush(this->sector());
                    if (err < 0) {
                        return err;
                    }

                    return 0;
                }
            }
        }

        auto err = forward(page_lock);
        if (err < 0) {
            return err;
        }
        if (err == 0) {
            break;
        }
    }

    assert(append<free_sectors_t>(page_lock, record, nullptr, 0u) >= 0);

    auto err = flush(page_lock);
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t free_sectors_chain::add_chain(dhara_sector_t head) {
    return add_free_sectors(free_sectors_t{ head });
}

int32_t free_sectors_chain::add_tree(tree_ptr_t tree) {
    return add_free_sectors(free_sectors_t{ tree.tail });
}

int32_t free_sectors_chain::dequeue(dhara_sector_t *sector) {
    assert(sector != nullptr);

    *sector = InvalidSector;

    auto page_lock = db().writing(head());

    assert(back_to_head(page_lock) >= 0);

    while (true) {
        for (auto iter = db().begin(); iter != db().end(); ++iter) {
            auto entry = iter->as<entry_t>();
            assert(entry != nullptr);

            if (entry->type == entry_type::FreeSectors) {
                auto fs = iter->as<free_sectors_t>();
                auto new_head = fs->head;

                int32_t err = 0;

                if (fs->head != InvalidSector) {
                    phydebugf("walk: free-sectors(chain): %d", fs->head);

                    free_sectors_chain chain{ *this, head_tail_t{ fs->head, InvalidSector }, "dequeue" };
                    err = chain.dequeue_sector(sector);
                    if (err < 0) {
                        return err;
                    }

                    new_head = chain.head();
                }

                if (err > 0) {
                    if (fs->head != new_head) {
                        phydebugf("walk: head changed %d vs %d", fs->head, new_head);

                        auto mutable_record = db().as_mutable<free_sectors_t>(*iter);

                        mutable_record->head = new_head;

                        page_lock.dirty();

                        err = page_lock.flush(this->sector());
                        if (err < 0) {
                            return err;
                        }
                    }

                    return 1;
                }
            }
        }

        auto err = forward(page_lock);
        if (err < 0) {
            return err;
        }
        if (err == 0) {
            break;
        }
    }

    return 0;
}

int32_t free_sectors_chain::write_header(page_lock &page_lock) {
    logged_task lt{ "fc-write-hdr", this->name() };

    assert_valid();

    db().emplace<free_chain_header_t>();

    page_lock.dirty();
    appendable(true);

    return 0;
}

int32_t free_sectors_chain::seek_end_of_buffer(page_lock & /*page_lock*/) {
    auto err = db().seek_end();
    if (err < 0) {
        return err;
    }

    appendable(true);

    return 0;
}

} // namespace phylum