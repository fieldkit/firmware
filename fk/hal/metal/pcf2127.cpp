#include "pcf2127.h"
#include "platform.h"

#if defined(__SAMD51__)

namespace fk {

FK_DECLARE_LOGGER("pcf2127");

constexpr uint8_t Pcf2127Address = 0x51;
constexpr uint8_t Pcf2127TimeRegister = 0x3;
constexpr uint8_t Pcf2127TimeSize = 10;

static_assert(Pcf2127TimeSize > 9, "pcf2127 time buffer too small");

static uint8_t bcd2bin(uint8_t val) {
    return val - 6 * (val >> 4);
}

static uint8_t bin2bcd(uint8_t val) {
    return val + 6 * (val / 10);
}

bool Pcf2127::configure() {
    return true;
}

bool Pcf2127::read(DateTime &time) {
    uint8_t data[Pcf2127TimeSize];
    if (!read_buffer(0, data, sizeof(data))) {
        return false;
    }

    uint8_t second = bcd2bin(data[3] & 0b01111111);
    uint8_t minute = bcd2bin(data[4] & 0b01111111);
    uint8_t hour = bcd2bin(data[5] & 0b00111111); // 24h mode, skipping 12h.
    uint8_t date = bcd2bin(data[6] & 0b00111111);
    // uint8_t weekday = data[7] & 0b00000111;
    uint8_t month = bcd2bin(data[8] & 0b00011111);
    uint16_t year = bcd2bin(data[9]) + 2000;

    time = DateTime{ year, month, date, hour, minute, second };

    return true;
}

bool Pcf2127::adjust(DateTime now) {
    auto bus = get_board()->i2c_core();

    uint8_t adjust_command[] = {
        Pcf2127TimeRegister,   //
        bin2bcd(now.second()), //
        bin2bcd(now.minute()), //
        bin2bcd(now.hour()),   //
        bin2bcd(now.day()),    //
        bin2bcd(0),            //
        bin2bcd(now.month()),  //
        bin2bcd(now.year() - 2000),
    };
    if (!I2C_CHECK(bus.write(Pcf2127Address, &adjust_command, sizeof(adjust_command)))) {
        return false;
    }

    return true;
}

bool Pcf2127::read_register(uint8_t reg, uint8_t &value) {
    auto bus = get_board()->i2c_core();

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
    auto bus = get_board()->i2c_core();

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