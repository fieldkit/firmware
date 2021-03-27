#pragma once

#include "sector_allocator.h"
#include "delimited_buffer.h"

namespace phylum {

class sector_chain {
private:
    static constexpr size_t ChainNameLength = 32;

private:
    sector_map *sectors_;
    sector_allocator *allocator_;
    delimited_buffer buffer_;
    dhara_sector_t head_{ InvalidSector };
    dhara_sector_t tail_{ InvalidSector };
    dhara_sector_t sector_{ InvalidSector };
    dhara_sector_t length_sectors_{ 0 };
    bool dirty_{ false };
    bool appendable_{ false };
    const char *prefix_{ "sector-chain" };
    char name_[ChainNameLength];

public:
    sector_chain(sector_map &sectors, sector_allocator &allocator, simple_buffer &&buffer, head_tail_t chain,
                 const char *prefix)
        : sectors_(&sectors), allocator_(&allocator), buffer_(std::move(buffer)), head_(chain.head), tail_(chain.tail),
          prefix_(prefix) {
        name("%s[unk]", prefix_);
    }

    sector_chain(sector_chain &other, head_tail_t chain, const char *prefix)
        : sectors_(other.sectors_), allocator_(other.allocator_), buffer_(other.sector_size()), head_(chain.head),
          tail_(chain.tail), prefix_(prefix) {
        name("%s[unk]", prefix_);
    }

    virtual ~sector_chain() {
    }

public:
    const char *name() const {
        return name_;
    }

    dhara_sector_t head() const {
        return head_;
    }

    dhara_sector_t tail() const {
        return tail_;
    }

    dhara_sector_t sector() const {
        return sector_;
    }

    dhara_sector_t length_sectors() const {
        return length_sectors_;
    }

    int32_t log();

    int32_t create_if_necessary();

    int32_t flush();

    int32_t back_to_head();

    bool valid() const {
        return head_ != 0 && head_ != InvalidSector && tail_ != 0 && tail_ != InvalidSector;
    }

protected:
    void name(const char *f, ...);

    sector_map *sectors() {
        return sectors_;
    }

    void dirty(bool value) {
        dirty_ = value;
    }

    bool dirty() {
        return dirty_;
    }

    void appendable(bool value) {
        appendable_ = value;
    }

    bool appendable() {
        return appendable_;
    }

    delimited_buffer const &buffer() {
        return buffer_;
    }

    sector_allocator &allocator() {
        return *allocator_;
    }

    delimited_buffer &db() {
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

    int32_t ensure_loaded() {
        assert_valid();

        if (sector_ == InvalidSector) {
            return forward();
        }

        return 0;
    }

    int32_t back_to_tail();

    int32_t forward();

    template <typename T>
    int32_t walk(T fn) {
        logged_task lt{ "sc-walk", name() };

        assert_valid();

        assert(!dirty());

        assert(back_to_head() >= 0);

        while (true) {
            auto err = forward();
            if (err < 0) {
                return err;
            }
            if (err == 0) {
                break;
            }
            for (auto &record : buffer_) {
                auto entry = record.as<entry_t>();
                assert(entry != nullptr);
                auto err = fn(entry, record);
                if (err < 0) {
                    return err;
                }
                if (err > 0) {
                    return err;
                }
            }
        }

        return 0;
    }

    template <typename T> T *header() {
        return db().begin()->as<T>();
    }

    int32_t load();

    int32_t grow_head();

    int32_t grow_tail();

    int32_t write_header_if_at_start();

    virtual int32_t seek_end_of_chain();

    virtual int32_t seek_end_of_buffer() = 0;

    virtual int32_t write_header() = 0;
};

} // namespace phylum
