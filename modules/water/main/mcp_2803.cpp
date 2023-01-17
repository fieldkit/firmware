#include "mcp_2803.h"

namespace fk {

#define MCP23008_IODIR   (0x00)
#define MCP23008_IPOL    (0x01)
#define MCP23008_GPINTEN (0x02)
#define MCP23008_DEFVAL  (0x03)
#define MCP23008_INTCON  (0x04)
#define MCP23008_IOCON   (0x05)
#define MCP23008_GPPU    (0x06)
#define MCP23008_INTF    (0x07)
#define MCP23008_INTCAP  (0x08)
#define MCP23008_GPIO    (0x09)
#define MCP23008_OLAT    (0x0A)

FK_DECLARE_LOGGER("mcp2803");

Mcp2803::Mcp2803(TwoWireWrapper &bus, uint8_t address) : bus_(bus), address_(address) {
}

Mcp2803::~Mcp2803() {
}

bool Mcp2803::configure(uint8_t iodir, uint8_t gppu, uint8_t gpio) {
    uint8_t buffer[] = {
        0x00,  // IODIR (Address)
        iodir, // IODIR
        0x00,  // IPOL
        0x00,  // GPINTEN
        0x00,  // DEFVAL
        0x00,  // INTCON
        0x00,  // IOCON
        gppu,  // GPPU
        0x00,  // INTF
        0x00,  // INTCAP
        gpio,  // GPIO
    };

    logdebug("configure(io=0x%x gpio=0x%x)", iodir, gpio);

    auto rv = bus_.write(address_, buffer, sizeof(buffer));
    if (!I2C_CHECK(rv)) {
        return false;
    }
    return true;
}

bool Mcp2803::configure(Mcp2803Config config) {
    return configure(config.iodir, config.gppu, config.gpio);
}

bool Mcp2803::read_gpio(uint8_t &gpio) {
    auto rv = bus_.read_register_u8(address_, MCP23008_GPIO, gpio);
    if (!I2C_CHECK(rv)) {
        return false;
    }
    return true;
}

} // namespace fk
