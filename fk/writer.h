#pragma once

#include "common.h"

namespace fk {

class Writable {
public:
    virtual int32_t write(uint8_t *buffer, size_t size) = 0;
};

}
