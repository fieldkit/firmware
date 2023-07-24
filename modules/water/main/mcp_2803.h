#pragma once

#include "common.h"
#include "hal/board.h"

namespace fk {

struct Mcp2803Config {
    uint8_t iodir;
    uint8_t gppu;
    uint8_t gpio;
};

class Mcp2803 {
private:
    TwoWireWrapper &bus_;
    uint8_t address_{ 0 };

public:
    Mcp2803(TwoWireWrapper &bus, uint8_t address);
    virtual ~Mcp2803();

public:
    bool configure(uint8_t iodir, uint8_t gppu, uint8_t gpio);

    bool configure(Mcp2803Config config);

    bool read_gpio(uint8_t &gpio);
};

} // namespace fk
