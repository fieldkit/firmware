#pragma once

#include "common.h"

namespace fk {

class Writable {
public:
    virtual int32_t write(uint8_t *buffer, size_t size) = 0;
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

}
