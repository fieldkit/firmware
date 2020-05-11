#pragma once

#include "io.h"

namespace fk {

class Base64Reader : public Reader {
private:
    Reader *target_;

public:
    explicit Base64Reader(Reader *target);

public:
    int32_t read(uint8_t *buffer, size_t size) override;
};

class Base64Writer : public Writer {
private:
    Writer *target_;

public:
    explicit Base64Writer(Writer *target);

public:
    int32_t write(uint8_t const *buffer, size_t size) override;
};

}
