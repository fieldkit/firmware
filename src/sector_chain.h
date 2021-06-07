#pragma once

#include "phylum.h"
#include "delimited_buffer.h"
#include "sector_allocator.h"
#include "working_buffers.h"
#include "paging_delimited_buffer.h"
#include "phyctx.h"

namespace phylum {

class sector_chain {
private:
    static constexpr size_t ChainNameLength = 32;

private:
    using buffer_type = paging_delimited_buffer;
    working_buffers *buffers_{ nullptr };
    sector_map *sectors_{ nullptr };
    sector_allocator *allocator_{ nullptr };
    buffer_type buffer_;
    dhara_sector_t head_{ InvalidSector };
    dhara_sector_t tail_{ InvalidSector };
    dhara_sector_t sector_{ InvalidSector };
    dhara_sector_t visited_sectors_{ 0 };
    bool appendable_{ false };
    const char *prefix_{ "sector-chain" };
    char name_[ChainNameLength];

public:
    sector_chain(phyctx pc, head_tail_t chain, const char *prefix)
        : buffers_(&pc.buffers_), sectors_(&pc.sectors_), allocator_(&pc.allocator_), buffer_(pc.buffers_, pc.sectors_), head_(chain.head),
          tail_(chain.tail), prefix_(prefix) {
        if (head_ == InvalidSector) {
            name("%s[unk]", prefix_);
        }
        else {
            name("%s[%d]", prefix_, head_);
        }
    }

    virtual ~sector_chain() {
    }

public:
    working_buffers &buffers() {
        return *buffers_;
    }

    phyctx pc() {
        return phyctx{ *buffers_, *sectors_, *allocator_ };
    }

    dhara_sector_t visited_sectors() const {
        return visited_sectors_;
    }

    void clear_visited_sectors() {
        visited_sectors_ = 0;
    }

    bool valid() const {
        return head_ != 0 && head_ != InvalidSector && tail_ != 0 && tail_ != InvalidSector;
    }

    dhara_sector_t head() const {
        return head_;
    }

    dhara_sector_t tail() const {
        return tail_;
    }

    head_tail_t chain() const {
        return  head_tail_t{ head_, tail_ };
    }

    int32_t log(bool graph = false);

    int32_t create_if_necessary();

    const char *name() const {
        return name_;
    }

    int32_t truncate();

    int32_t flush();

    int32_t dequeue_sector(dhara_sector_t *sector);

protected:
    int32_t flush(page_lock &page_lock);

    void name(const char *f, ...);

    dhara_sector_t sector() const {
        return sector_;
    }

    sector_map *sectors() {
        return sectors_;
    }

    void appendable(bool value) {
        appendable_ = value;
    }

    bool appendable() {
        return appendable_;
    }

    sector_allocator &allocator() {
        return *allocator_;
    }

    buffer_type &db() {
        return buffer_;
    }

    size_t sector_size() const {
        return buffer_.size();
    }

    void head(dhara_sector_t sector) {
        assert(sector != InvalidSector);
        head_ = sector;
        if (tail_ == InvalidSector) {
            tail(sector);
        }
    }

    void tail(dhara_sector_t sector) {
        assert(sector != InvalidSector);
        tail_ = sector;
        if (head_ == InvalidSector) {
            head(sector);
        }
    }

    void sector(dhara_sector_t sector) {
        sector_ = sector;
        if (sector_ == InvalidSector) {
            name("%s[unk]", prefix_, sector_);
        }
        else {
            name("%s[%d]", prefix_, sector_);
        }
    }

    int32_t assert_valid() {
        assert(head_ != InvalidSector);
        return 0;
    }

    int32_t ensure_loaded(page_lock &page_lock) {
        assert_valid();

        if (sector_ == InvalidSector) {
            return forward(page_lock);
        }

        return 0;
    }

    int32_t back_to_head(page_lock &page_lock);

    int32_t prepare_sector(page_lock &lock, dhara_sector_t previous_sector, bool preserve_header);

    int32_t forward(page_lock &page_lock);

    template <typename WalkFn, typename LoadFn>
    int32_t walk(page_lock &page_lock, WalkFn walk_fn, LoadFn load_fn) {
        // logged_task lt{ "sc-walk", name() };

        assert_valid();

        while (true) {
            for (auto record_ptr : buffer_) {
                auto entry = record_ptr.as<entry_t>();
                assert(entry != nullptr);
                auto err = walk_fn(page_lock, entry, record_ptr);
                if (err < 0) {
                    return err;
                }
                if (err > 0) {
                    return err;
                }
            }

            auto previous_sector = sector();

            auto err = forward(page_lock);
            if (err < 0) {
                return err;
            }
            if (err == 0) {
                break;
            }

            err = load_fn(previous_sector, sector());
            if (err < 0) {
                break;
            }
        }

        return 0;
    }

    template <typename WalkFn>
    int32_t walk(WalkFn fn) {
        return walk(fn, [](dhara_sector_t, dhara_sector_t) {
            return 0;
        });
    }

    template <typename WalkFn, typename LoadFn>
    int32_t walk(WalkFn walk_fn, LoadFn load_fn) {
        // logged_task lt{ "sc-walk", name() };

        assert_valid();

        auto page_lock = db().reading(head());

        assert(back_to_head(page_lock) >= 0);

        auto err = load_fn(InvalidSector, sector());
        if (err < 0) {
            return err;
        }

        return walk(page_lock, walk_fn, load_fn);
    }

    int32_t load(page_lock &page_lock);

    int32_t grow_tail(page_lock &page_lock);

    int32_t write_header_if_at_start(page_lock &page_lock);

    virtual int32_t seek_end_of_chain(page_lock &page_lock);

    virtual int32_t seek_end_of_buffer(page_lock &page_lock) = 0;

    virtual int32_t write_header(page_lock &page_lock) = 0;
};

} // namespace phylum
