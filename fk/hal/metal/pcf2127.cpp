#include "pcf2127.h"
#include "platform.h"

#if defined(__SAMD51__)

namespace fk {

FK_DECLARE_LOGGER("pcf2127");

constexpr uint8_t Pcf2127Address = 0x51;

Pcf2127::Pcf2127(AcquireTwoWireBus *acquire_bus) : acquire_bus_(acquire_bus) {
}

bool Pcf2127::begin() {
    auto bus = acquire_bus_->acquire();

    bus.begin();

    uint8_t data[32];

    if (!read_buffer(0, data, sizeof(data))) {
        return false;
    }

    fk_logs_dump_memory_u8("pcf2127", data, sizeof(data));

    return true;
}

bool Pcf2127::read_register(uint8_t reg, uint8_t &value) {
    auto bus = acquire_bus_->acquire();

    if (!I2C_CHECK(bus.write(Pcf2127Address, &reg, sizeof(reg)))) {
        return false;
    }

    uint8_t data[1];

    if (!I2C_CHECK(bus.read(Pcf2127Address, data, sizeof(data)))) {
        return false;
    }

    value = data[0];

    return true;
}

bool Pcf2127::read_buffer(uint8_t reg, uint8_t *value, size_t size) {
    auto bus = acquire_bus_->acquire();

    if (!I2C_CHECK(bus.write(Pcf2127Address, &reg, sizeof(reg)))) {
        return false;
    }

    if (!I2C_CHECK(bus.read(Pcf2127Address, value, size))) {
        return false;
    }

    return true;
}

} // namespace fk

#endif