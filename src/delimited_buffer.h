#pragma once

#include "phylum.h"
#include "simple_buffer.h"

namespace phylum {

struct written_record {
    simple_buffer view;
    size_t size_of_record;
    size_t size_with_delimiter;
    size_t position;

    template <typename T>
    T *as() {
        return reinterpret_cast<T *>(view.cursor());
    }

    template<typename T>
    simple_buffer data() {
        auto p = (uint8_t *)view.cursor() + sizeof(T);
        return simple_buffer{ p, size_of_record - sizeof(T) };
    }
};

/**
 * This has the mechanics to allow us to overlap records across sector
 * boundaries. I'm torn on introducing this complexity though because
 * what happens is changing those records becomes a multiblock
 * juggle. If your flash memory has small pages relative to the data
 * being stored this could be a problem.
 *
 * This may be acceptable for certain kinds of records, though and
 * that may be worth the effort. For example, some things will never
 * be modified.
 */
class delimited_buffer {
private:
    simple_buffer buffer_;
    size_t offset_{ 0 };

public:
    delimited_buffer(size_t size, size_t offset = 0) : buffer_(size), offset_(offset) {
    }

    delimited_buffer(simple_buffer &&buffer, size_t offset = 0) : buffer_(std::move(buffer)), offset_(offset) {
    }

public:
    template <typename T>
    sector_offset_t append(T &record) {
        return append(&record, sizeof(T));
    }

    template <typename T>
    sector_offset_t append(T &record, uint8_t const *buffer, size_t size) {
        sector_offset_t start_position{ 0 };
        auto allocd = (uint8_t *)reserve(sizeof(T) + size, start_position);
        memcpy(allocd, &record, sizeof(T));
        memcpy(allocd + sizeof(T), buffer, size);
        return start_position;
    }

    template <typename T, class... Args>
    sector_offset_t emplace(Args &&... args) {
        sector_offset_t start_position{ 0 };
        auto allocd = reserve(sizeof(T), start_position);
        new (allocd) T(std::forward<Args>(args)...);
        return start_position;
    }

    sector_offset_t append(void const *source, size_t length) {
        sector_offset_t start_position{ 0 };
        memcpy(reserve(length, start_position), source, length);
        return start_position;
    }

    template<typename T>
    struct appended_record {
        sector_offset_t position;
        T *record;
    };

    template<typename T>
    appended_record<T> reserve() {
        sector_offset_t start_position{ 0 };
        auto size = sizeof(T);
        auto alloc = reserve(size, start_position);
        bzero(alloc, size);
        auto ptr = new (alloc) T();
        return appended_record<T>{ start_position, ptr };
    }

    /**
     * This reserves 0 bytes, which ends up inserting a 0 byte length
     * and that acts a NULL terminator.
     */
    sector_offset_t terminate() {
        sector_offset_t start_position{ 0 };
        (uint8_t *)reserve(0, start_position);
        return start_position;
    }

    template <typename T>
    bool room_for() {
        return room_for(sizeof(T));
    }

    bool room_for(size_t length) {
        return buffer_.room_for(varint_encoding_length(length) + length);
    }

    template <typename T>
    int32_t unsafe_all(T fn) {
        return buffer_.unsafe_all<T>(fn);
    }

    template <typename T>
    int32_t unsafe_at(T fn) {
        return buffer_.unsafe_at<T>(fn);
    }

    simple_buffer &read_view() {
        return buffer_;
    }

    simple_buffer &write_view() {
        return buffer_;
    }

    int32_t skip_end() {
        return buffer_.skip_end();
    }

    int32_t skip(size_t bytes) {
        return buffer_.skip(bytes);
    }

    int32_t constrain(size_t bytes) {
        return buffer_.constrain(bytes);
    }

private:
    void *reserve(size_t length, sector_offset_t &start_position);

public:
    class iterator {
    private:
        simple_buffer view_;
        written_record record_;

    public:
        uint8_t *ptr() {
            return view_.ptr();
        }

        size_t position() {
            return view_.position();
        }

    public:
        iterator(simple_buffer &&view) : view_(std::move(view)) {
            if (view_.valid()) {
                uint32_t offset = 0u;
                if (!view_.try_read(offset)) {
                    view_ = view_.end_view();
                } else {
                    if (!read()) {
                        view_ = view_.end_view();
                    }
                }
            }
        }

    private:
        bool read() {
            assert(view_.valid());

            // Position of the delimiter, rather than the record.
            auto record_position = position();

            uint32_t maybe_record_length = 0u;
            if (!view_.try_read(maybe_record_length)) {
                record_ = written_record{};
                return false;
            }

            if (maybe_record_length == 0) {
                record_ = written_record{};
                return false;
            }

            auto delimiter_overhead = varint_encoding_length(maybe_record_length);

            // Clear record and fill in the details we have.
            record_ = written_record{};
            record_.view = view_;
            record_.position = record_position;
            record_.size_of_record = maybe_record_length;
            record_.size_with_delimiter = maybe_record_length + delimiter_overhead;

            return true;
        }

    public:
        iterator &operator++() {
            // Advance to the record after this one, we only adjust
            // position_, then we do a read and see if we have a
            // legitimate record length.
            view_.skip(record_.size_of_record);
            if (!view_.valid()) {
                view_ = view_.end_view();
                return *this;
            }
            if (!read()) {
                view_ = view_.end_view();
            }
            return *this;
        }

        bool operator!=(const iterator &other) const {
            return view_.position() != other.view_.position();
        }

        bool operator==(const iterator &other) const {
            return view_.position() == other.view_.position();
        }

        written_record &operator*() {
            return record_;
        }

        written_record *operator->() {
            return &record_;
        }
    };

public:
    iterator begin() const {
        return iterator(buffer_.begin_view());
    }

    iterator end() const {
        return iterator(buffer_.end_view());
    }

    int32_t seek_end() {
        auto iter = begin();
        while (iter != end()) {
            buffer_.position(iter.position() + iter->size_of_record);
            iter = ++iter;
        }
        return 0;
    }

    int32_t seek_once() {
        auto iter = begin();
        while (iter != end()) {
            buffer_.position(iter.position() + iter->size_of_record);
            return 0;
        }
        return -1;
    }

    size_t size() const {
        return buffer_.size();
    }

    size_t position() const {
        return buffer_.position();
    }

    bool empty() const {
        return buffer_.position() == 0;
    }

    bool at_start() const {
        return empty();
    }

    void position(size_t position) {
        buffer_.position(position);
    }

    size_t available() const {
        return buffer_.available();
    }

    void clear(uint8_t value = 0xff) {
        buffer_.clear(value);
    }

    void rewind() {
        buffer_.rewind();
    }
};

} // namespace phylum
