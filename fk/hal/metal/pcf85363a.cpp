#include "hal/metal/pcf85363a.h"
#include "hal/clock.h"

namespace fk {

FK_DECLARE_LOGGER("pcf85363a");

#define CTRL_OFFSET     0x24
#define CTRL_OSCILLATOR 0x25
#define CTRL_BATTERY    0x26
#define CTRL_PIN_IO     0x27
#define CTRL_FUNCTION   0x28
#define CTRL_INTA_EN    0x29
#define CTRL_INTB_EN    0x2a
#define CTRL_FLAGS      0x2b
#define CTRL_RAMBYTE    0x2c
#define CTRL_WDOG       0x2d
#define CTRL_STOP_EN    0x2e
#define CTRL_RESETS     0x2f
#define CTRL_RAM        0x40

#define FLAGS_TSR1F BIT(0)
#define FLAGS_TSR2F BIT(1)
#define FLAGS_TSR3F BIT(2)
#define FLAGS_BSF   BIT(3)
#define FLAGS_WDF   BIT(4)
#define FLAGS_A1F   BIT(5)
#define FLAGS_A2F   BIT(6)
#define FLAGS_PIF   BIT(7)

#define NVRAM_SIZE   0x40
#define RESET_CPR    0xa4
#define STOP_EN_STOP BIT(0)

static uint8_t bcd2bin(uint8_t val) {
    return val - 6 * (val >> 4);
}

static uint8_t bin2bcd(uint8_t val) {
    return val + 6 * (val / 10);
}

bool Pcf85363a::configure() {
    auto bus = get_board()->i2c_core();

    uint8_t bsm;
    if (!I2C_CHECK(bus.read_register_u8(Address, 0x26, bsm))) {
        return false;
    }

    if (bsm != 0) {
        loginfo("fixing battery switch mode = 0x%x", bsm);
        if (!I2C_CHECK(bus.write_register_u8(Address, 0x26, 0x00))) {
            return false;
        }
    }

    return true;
}

bool Pcf85363a::adjust(DateTime now) {
    auto bus = get_board()->i2c_core();

    uint8_t adjust_command[] = {
        CTRL_STOP_EN,
        STOP_EN_STOP,
        RESET_CPR,
        0,
        bin2bcd(now.second()),
        bin2bcd(now.minute()),
        bin2bcd(now.hour()),
        bin2bcd(now.day()),
        bin2bcd(0),
        bin2bcd(now.month()),
        bin2bcd(now.year() - 2000),
    };
    if (!I2C_CHECK(bus.write(Address, &adjust_command, sizeof(adjust_command)))) {
        return false;
    }

    uint8_t resume_command[] = {
        CTRL_STOP_EN,
        0,
    };
    if (!I2C_CHECK(bus.write(Address, &resume_command, sizeof(resume_command)))) {
        return false;
    }

    return true;
}

bool Pcf85363a::read(DateTime &time) {
    auto bus = get_board()->i2c_core();

    uint8_t data[8];
    if (!I2C_CHECK(bus.read_register_buffer(Address, 0x00, data, sizeof(data)))) {
        return false;
    }

    auto os_flag = data[1] & 0b10000000;
    if (os_flag) {
        loginfo("possible accuracy error!");
    }

    time = DateTime{ (uint16_t)(bcd2bin(data[7]) + 2000), bcd2bin(data[6] & 0b00011111), bcd2bin(data[4] & 0b00111111),
                     bcd2bin(data[3] & 0b00111111),       bcd2bin(data[2] & 0b01111111), bcd2bin(data[1] & 0b01111111) };

    return true;
}

} // namespace fk