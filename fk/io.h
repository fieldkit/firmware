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
    uint8_t buffer_[128];
    size_t buffer_size_{ 128 };
    size_t position_{ 0 };
    int32_t return_value_{ 0 };

public:
    BufferedWriter(Writer *writer);
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
    uint8_t buffer_[128];
    size_t buffer_size_{ 128 };
    size_t position_{ 0 };
    size_t bytes_read_{ 0 };

public:
    BufferedReader(Reader *reader);
    virtual ~BufferedReader();

public:
    int32_t read(uint8_t *buffer, size_t size) override;

};

pb_ostream_t pb_ostream_from_writable(Writer *s);

pb_istream_t pb_istream_from_readable(Reader *s);

}
