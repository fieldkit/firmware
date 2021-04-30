#pragma once

#include "common.h"
#include "hal/board.h"

namespace fk {

class Mcp2803 {
private:
    uint8_t address_{ 0 };

public:
    Mcp2803(uint8_t address);
    virtual ~Mcp2803();

public:
    bool begin(TwoWireWrapper &bus, uint8_t iodir, uint8_t gppu, uint8_t gpio);

    bool read_gpio(TwoWireWrapper &bus, uint8_t &gpio);

};

}
