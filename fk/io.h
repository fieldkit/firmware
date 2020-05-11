#pragma once

#include <pb_decode.h>
#include <pb_encode.h>

#include "common.h"

namespace fk {

class Writer {
public:
    virtual int32_t write(uint8_t const *buffer, size_t size) = 0;
};

class Reader {
public:
    virtual int32_t read(uint8_t *buffer, size_t size) = 0;
};

class BufferedWriter : public Writer {
private:
    Writer *writer_;
    uint8_t *buffer_;
    size_t buffer_size_;
    size_t position_{ 0 };
    int32_t return_value_{ 0 };

public:
    BufferedWriter(Writer *writer, uint8_t *buffer, size_t size);
    virtual ~BufferedWriter();

public:
    int32_t write(uint8_t const *buffer, size_t size) override;
    int32_t write(const char *s, ...);
    int32_t write(char c);
    int32_t flush();

};

class BufferedReader : public Reader {
private:
    Reader *reader_;
    uint8_t *buffer_;
    size_t buffer_size_;
    size_t position_{ 0 };
    size_t bytes_read_{ 0 };

public:
    BufferedReader(Reader *reader, uint8_t *buffer, size_t size);
    virtual ~BufferedReader();

public:
    int32_t read(uint8_t *buffer, size_t size) override;

};

template<size_t Size>
class StackBufferedWriter : public BufferedWriter {
private:
    uint8_t buffer_[Size]{ };

public:
    explicit StackBufferedWriter(Writer *writer) : BufferedWriter(writer, buffer_, Size) {
    }

};

class Buffer {
private:
    uint8_t *ptr_;
    size_t size_;
    size_t position_;

public:
    Buffer(uint8_t *ptr, size_t size);

public:
    const uint8_t *ptr() const {
        return ptr_;
    }

    size_t position() const {
        return position_;
    }

public:
    void write(char c);
    bool full() const;
    void clear();

};

pb_ostream_t pb_ostream_from_writable(Writer *s);

pb_istream_t pb_istream_from_readable(Reader *s);

}
