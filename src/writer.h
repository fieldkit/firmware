#pragma once

#include "phylum.h"
#include "simple_buffer.h"
#include "blake2b.h"

namespace phylum {

class io_writer {
public:
    virtual int32_t write(uint8_t const *data, size_t size) = 0;

    int32_t write(char const *str) {
        return write((uint8_t *)str, strlen(str));
    }

    int32_t write(char const *str, size_t size) {
        return write((uint8_t *)str, size);
    }

};

class buffer_writer : public io_writer {
private:
    write_buffer &buffer_;

public:
    buffer_writer(write_buffer &buffer) : buffer_(buffer) {
    }

public:
    int32_t write(uint8_t const *data, size_t size) override {
        return buffer_.fill_from_buffer_ptr(data, size, [](simple_buffer&) -> int32_t {
            return 0;
        });
    }

};

class buffering_writer : public io_writer {
private:
    io_writer *target_{ nullptr };
    write_buffer &buffer_;

public:
    buffering_writer(io_writer *target, write_buffer &buffer) : target_(target), buffer_(buffer) {
    }

public:
    int32_t write(uint8_t const *data, size_t size) override {
        auto err = buffer_.fill_from_buffer_ptr(data, size, [=](simple_buffer &rb) -> int32_t {
            return target_->write(rb.ptr(), rb.position());
        });
        return err;
    }

    int32_t flush() {
        return target_->write(buffer_.ptr(), buffer_.position());
    }

};

class noop_writer : public io_writer {
private:
    uint32_t remaining_{ UINT32_MAX };

public:
    noop_writer(uint32_t remaining = UINT32_MAX) : remaining_(remaining) {
    }

public:
    int32_t write(uint8_t const *data, size_t size) override;

};

class varint_decoder : public io_writer {
private:
    int32_t width_{ 0 };
    int32_t bytes_read_{ 0 };
    uint32_t value_{ 0 };
    bool done_{ false };

public:
    uint32_t value() const {
        return value_;
    }

    bool done() const {
        return done_;
    }

    int32_t bytes_read() const {
        return bytes_read_;
    }

public:
    int32_t write(uint8_t const *data, size_t size) override;
};

constexpr size_t HashSize = 32;

class blake2b_writer : public io_writer {
private:
    io_writer *target_{ nullptr };
    BLAKE2b b2b_;

public:
    blake2b_writer(io_writer *target);

public:
    int32_t write(uint8_t const *data, size_t size) override;

public:
    void finalize(void *hash, size_t buffer_length);
};

} // namespace phylum
