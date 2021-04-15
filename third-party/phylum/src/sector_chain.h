#pragma once

#include "phylum.h"
#include "delimited_buffer.h"
#include "sector_allocator.h"
#include "working_buffers.h"
#include "paging_delimited_buffer.h"

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
    dhara_sector_t length_sectors_{ 0 };
    bool appendable_{ false };
    const char *prefix_{ "sector-chain" };
    char name_[ChainNameLength];

public:
    sector_chain(working_buffers &buffers, sector_map &sectors, sector_allocator &allocator, head_tail_t chain, const char *prefix)
        : buffers_(&buffers), sectors_(&sectors), allocator_(&allocator), buffer_(buffers, sectors), head_(chain.head),
          tail_(chain.tail), prefix_(prefix) {
        name("%s[unk]", prefix_);
    }

    sector_chain(sector_chain &other, head_tail_t chain, const char *prefix)
        : buffers_(other.buffers_), sectors_(other.sectors_), allocator_(other.allocator_),
          buffer_(*other.buffers_, *other.sectors_), head_(chain.head), tail_(chain.tail), prefix_(prefix) {
        name("%s[unk]", prefix_);
    }

    virtual ~sector_chain() {
    }

public:
    working_buffers &buffers() {
        return *buffers_;
    }

    dhara_sector_t length_sectors() const {
        return length_sectors_;
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

    int32_t log();

    int32_t create_if_necessary();

    const char *name() const {
        return name_;
    }

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
        head_ = sector;
        if (tail_ == InvalidSector) {
            tail(sector);
        }
    }

    void tail(dhara_sector_t sector) {
        tail_ = sector;
        if (head_ == InvalidSector) {
            head(sector);
        }
    }

    void sector(dhara_sector_t sector) {
        sector_ = sector;
        name("%s[%d]", prefix_, sector_);
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

    int32_t forward(page_lock &page_lock);

    template <typename T>
    int32_t walk(page_lock &page_lock, T fn) {
        logged_task lt{ "sc-walk", name() };

        assert_valid();

        while (true) {
            for (auto &record_ptr : buffer_) {
                auto entry = record_ptr.as<entry_t>();
                assert(entry != nullptr);
                auto err = fn(page_lock, entry, record_ptr);
                if (err < 0) {
                    return err;
                }
                if (err > 0) {
                    return err;
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
    template <typename T>
    int32_t walk(T fn) {
        logged_task lt{ "sc-walk", name() };

        assert_valid();

        auto page_lock = db().reading(head());

        assert(back_to_head(page_lock) >= 0);

        return walk(page_lock, fn);
    }

    int32_t load(page_lock &page_lock);

    int32_t grow_tail(page_lock &page_lock);

    int32_t write_header_if_at_start(page_lock &page_lock);

    virtual int32_t seek_end_of_chain(page_lock &page_lock);

    virtual int32_t seek_end_of_buffer(page_lock &page_lock) = 0;

    virtual int32_t write_header(page_lock &page_lock) = 0;
};

} // namespace phylum
