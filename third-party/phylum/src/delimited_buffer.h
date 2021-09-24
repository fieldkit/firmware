#pragma once

#include "phylum.h"
#include "simple_buffer.h"

namespace phylum {

class delimited_buffer;

struct record_ptr {
private:
    read_buffer buffer_;
    size_t size_of_record_{ 0 };

public:
    size_t position() const {
        return buffer_.position();
    }

    size_t delimited_size() const {
        return size_of_record_;
    }

    size_t size_of_record() const {
        return buffer_.available();
    }

    size_t start_of_record() const {
        auto size_overhead = varint_encoding_length(size_of_record_);
        return buffer_.position() + size_overhead;
    }

public:
    record_ptr() {
    }

    record_ptr(read_buffer buffer, size_t size_of_record) : buffer_(std::move(buffer)), size_of_record_(size_of_record) {
    }

public:
    template <typename DataType, typename T>
    int32_t read_data(T fn) const {
        auto size_overhead = varint_encoding_length(size_of_record_);
        read_buffer data_buffer{ buffer_.cursor() + sizeof(DataType) + size_overhead,
                                 size_of_record() - sizeof(DataType) - size_overhead };
        return fn(std::move(data_buffer));
    }

    template <typename TRecord>
    TRecord const *as() {
        auto size_overhead = varint_encoding_length(size_of_record_);
        return reinterpret_cast<TRecord const *>(buffer_.cursor() + size_overhead);
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

protected:
    delimited_buffer() {
    }

public:
    delimited_buffer(simple_buffer &&buffer) : buffer_(std::move(buffer)) {
    }

    delimited_buffer(delimited_buffer &&other)
        : buffer_(std::exchange(other.buffer_, simple_buffer{ })) {
    }

    virtual ~delimited_buffer() {
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
        if (buffer != nullptr) {
            memcpy(allocd + sizeof(T), buffer, size);
        }
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
        ensure_valid();
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
        ensure_valid();
        return buffer_.unsafe_all<T>(fn);
    }

    template <typename T>
    int32_t unsafe_forever(T fn) {
        ensure_valid();
        return buffer_.unsafe_forever<T>(fn);
    }

    template <typename T>
    int32_t read_to_end(T fn) const {
        ensure_valid();
        return buffer_.read_to_end<T>(fn);
    }

    template <typename T>
    int32_t read_to_position(T fn) const {
        ensure_valid();
        return buffer_.read_to_position<T>(fn);
    }

    read_buffer to_read_buffer() const {
        ensure_valid();
        return read_buffer{ buffer_.ptr(), buffer_.size(), buffer_.position() };
    }

    template <typename T>
    T const *header() const {
        ensure_valid();
        auto rp = *begin();
        return rp.as<T>();
    }

    template<typename THeader, typename MutateHeaderFunction>
    int32_t write_header(MutateHeaderFunction fn) {
        ensure_valid();
        auto rp = *begin();
        return fn(as_mutable<THeader>(rp));
    }

    template<typename WriteFunction>
    int32_t write_view(WriteFunction fn) {
        ensure_valid();
        return unsafe_all([&](uint8_t *ptr, size_t size) -> int32_t {
            return fn(write_buffer(ptr, size, position()));
        });
    }

    int32_t skip_end() {
        ensure_valid();
        return buffer_.skip_end();
    }

    int32_t skip(size_t bytes) {
        ensure_valid();
        return buffer_.skip(bytes);
    }

    int32_t constrain(size_t bytes) {
        ensure_valid();
        return buffer_.constrain(bytes);
    }

    int32_t seek_end() {
        ensure_valid();
        auto iter = begin();
        while (iter != end()) {
            buffer_.position(iter.position() + iter.size_of_record());
            iter = ++iter;
        }
        return 0;
    }

    int32_t seek_once() {
        ensure_valid();
        auto iter = begin();
        while (iter != end()) {
            buffer_.position(iter.position() + iter.size_of_record());
            return 0;
        }
        return -1;
    }

    size_t size() const {
        assert(buffer_.size() > 0);
        return buffer_.size();
    }

    size_t position() const {
        ensure_valid();
        return buffer_.position();
    }

    bool empty() const {
        ensure_valid();
        return buffer_.position() == 0;
    }

    bool at_start() const {
        ensure_valid();
        return empty();
    }

    void position(size_t position) {
        ensure_valid();
        buffer_.position(position);
    }

    size_t available() const {
        ensure_valid();
        return buffer_.available();
    }

    void clear(uint8_t value = 0xff) {
        ensure_valid();
        buffer_.clear(value);
    }

    void rewind() {
        ensure_valid();
        buffer_.rewind();
    }

    void debug(const char *prefix, size_t bytes) {
        phydebug_dump_memory(prefix, ptr(), bytes);
    }

    template<typename T>
    T* as_mutable(record_ptr &record_ptr) {
        return reinterpret_cast<T*>(buffer_.ptr() + record_ptr.start_of_record());
    }

protected:
    void ptr(uint8_t *ptr, size_t size) {
        buffer_.ptr(ptr, size);
    }

    uint8_t *ptr() {
        return buffer_.ptr();
    }

public:
    class iterator {
    private:
        read_buffer buffer_;
        int32_t record_size_{ -1 };

    public:
        size_t position() {
            return buffer_.position();
        }

        int32_t size_of_record() {
            return record_size_;
        }

    public:
        iterator(read_buffer buffer) : buffer_(std::move(buffer)) {
            if (buffer_.valid()) {
                uint32_t offset = 0u;
                if (!buffer_.try_read(offset)) {
                    buffer_ = buffer_.end_view();
                } else {
                    if (!read()) {
                        buffer_ = buffer_.end_view();
                    }
                }
            }
        }

    private:
        bool read() {
            assert(buffer_.valid());

            uint32_t maybe_record_length = 0u;
            if (!buffer_.try_read(maybe_record_length)) {
                record_size_ = -1;
                return false;
            }

            record_size_ = maybe_record_length;
            return true;
        }

    public:
        iterator &operator++() {
            // Advance to the record after this one, we only adjust
            // position_, then we do a read and see if we have a
            // legitimate record length.
            auto size_of_record = record_size_;
            if (size_of_record == 0) {
                buffer_ = buffer_.end_view();
                return *this;
            }
            buffer_.skip(size_of_record);
            if (!buffer_.valid()) {
                buffer_ = buffer_.end_view();
                return *this;
            }
            if (!read()) {
                buffer_ = buffer_.end_view();
            }
            return *this;
        }

        bool operator!=(const iterator &other) const {
            return buffer_.position() != other.buffer_.position();
        }

        bool operator==(const iterator &other) const {
            return buffer_.position() == other.buffer_.position();
        }

        record_ptr operator*() {
            auto delimiter_overhead = varint_encoding_length(record_size_);
            auto position_after_delimiter = buffer_.position();
            auto position_before_delimiter = position_after_delimiter - delimiter_overhead;
            auto position_end_of_record = position_after_delimiter + record_size_;
            read_buffer record_buffer( buffer_.ptr(), position_end_of_record, position_before_delimiter);
            return record_ptr{ std::move(record_buffer), (size_t)record_size_ };
        }
    };

public:
    iterator begin() const {
        ensure_valid();
        return iterator(buffer_.begin_view());
    }

    iterator end() const {
        ensure_valid();
        return iterator(buffer_.end_view());
    }

private:
    void *reserve(size_t length, sector_offset_t &start_position);

protected:
    virtual void ensure_valid() const {
        assert(buffer_.ptr() != nullptr);
    }

};

} // namespace phylum
