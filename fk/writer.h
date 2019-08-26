#pragma once

#include <pb_decode.h>
#include <pb_encode.h>

#include "common.h"

namespace fk {

class Writable {
public:
    virtual int32_t write(uint8_t const *buffer, size_t size) = 0;
};

class BufferedWriter {
private:
    uint8_t buffer_[128];
    size_t buffer_size_{ 128 };
    size_t position_{ 0 };
    int32_t return_value_{ 0 };
    Writable *writer_;

public:
    BufferedWriter(Writable *writer);
    virtual ~BufferedWriter();

public:
    int32_t write(const char *s, ...);
    int32_t write(char c);
    int32_t flush();

};

class Readable {
public:
    virtual int32_t read(uint8_t *buffer, size_t size) = 0;
};

class BufferedReader : public Readable {
private:
    uint8_t buffer_[128];
    size_t buffer_size_{ 128 };
    size_t position_{ 0 };
    int32_t return_value_{ 0 };
    Readable *reader_;

public:
    BufferedReader(Readable *reader);
    virtual ~BufferedReader();

public:
    int32_t read(uint8_t *buffer, size_t size) override;

};

class Base64Reader : public Readable {
private:
    Readable *target_;

public:
    Base64Reader(Readable *target);

public:
    int32_t read(uint8_t *buffer, size_t size) override;
};

class Base64Writer : public Writable {
private:
    Writable *target_;

public:
    Base64Writer(Writable *target);

public:
    int32_t write(uint8_t const *buffer, size_t size) override;
};

pb_ostream_t pb_ostream_from_writable(Writable *s);

pb_istream_t pb_istream_from_readable(Readable *s);

}
