#pragma once

#include "io.h"

namespace fk {

class HexReader : public Reader {
private:
    Reader *target_;

public:
    explicit HexReader(Reader *target);

public:
    int32_t read(uint8_t *buffer, size_t size) override;
};

class HexWriter : public Writer {
private:
    Writer *target_;

public:
    explicit HexWriter(Writer *target);

public:
    int32_t write(uint8_t const *buffer, size_t size) override;
};

}
