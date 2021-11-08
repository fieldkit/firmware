#include "mpl3115a2.h"
#include "common.h"
#include "platform.h"

namespace fk {

#define MPL3115A2_I2C_ADDRESS           (0x60)
#define MPL3115A2_REGISTER_STATUS       (0x00)
#define MPL3115A2_REGISTER_PRESSURE_MSB (0x01)
#define MPL3115A2_REGISTER_PRESSURE_CSB (0x02)
#define MPL3115A2_REGISTER_PRESSURE_LSB (0x03)
#define MPL3115A2_REGISTER_TEMP_MSB     (0x04)
#define MPL3115A2_REGISTER_TEMP_LSB     (0x05)
#define MPL3115A2_REGISTER_DR_STATUS    (0x06)
#define MPL3115A2_OUT_P_DELTA_MSB       (0x07)
#define MPL3115A2_OUT_P_DELTA_CSB       (0x08)
#define MPL3115A2_OUT_P_DELTA_LSB       (0x09)
#define MPL3115A2_OUT_T_DELTA_MSB       (0x0A)
#define MPL3115A2_OUT_T_DELTA_LSB       (0x0B)
#define MPL3115A2_WHOAMI                (0x0C)
#define MPL3115A2_BAR_IN_MSB            (0x14)
#define MPL3115A2_BAR_IN_LSB            (0x15)
#define MPL3115A2_WHOAMI_EXPECTED       (0xC4)

#define MPL3115A2_REGISTER_STATUS_TDR  (0x02)
#define MPL3115A2_REGISTER_STATUS_PDR  (0x04)
#define MPL3115A2_REGISTER_STATUS_PTDR (0x08)

#define MPL3115A2_CTRL_REG1 (0x26)
#define MPL3115A2_CTRL_REG2 (0x27)
#define MPL3115A2_CTRL_REG3 (0x28)
#define MPL3115A2_CTRL_REG4 (0x29)
#define MPL3115A2_CTRL_REG5 (0x2A)

#define MPL3115A2_CTRL_REG1_SBYB (0x01)
#define MPL3115A2_CTRL_REG1_OST  (0x02)
#define MPL3115A2_CTRL_REG1_RST  (0x04)
#define MPL3115A2_CTRL_REG1_RAW  (0x40)
#define MPL3115A2_CTRL_REG1_ALT  (0x80)
#define MPL3115A2_CTRL_REG1_BAR  (0x00)

#define MPL3115A2_PT_DATA_CFG       (0x13)
#define MPL3115A2_PT_DATA_CFG_TDEFE (0x01)
#define MPL3115A2_PT_DATA_CFG_PDEFE (0x02)
#define MPL3115A2_PT_DATA_CFG_DREM  (0x04)

#define MPL3115A2_CTRL_REG1_OS1   (0x00)
#define MPL3115A2_CTRL_REG1_OS2   (0x08)
#define MPL3115A2_CTRL_REG1_OS4   (0x10)
#define MPL3115A2_CTRL_REG1_OS8   (0x18)
#define MPL3115A2_CTRL_REG1_OS16  (0x20)
#define MPL3115A2_CTRL_REG1_OS32  (0x28)
#define MPL3115A2_CTRL_REG1_OS64  (0x30)
#define MPL3115A2_CTRL_REG1_OS128 (0x38)

FK_DECLARE_LOGGER("mpl3115a2");

typedef union {
    struct {
        uint8_t SBYB : 1;
        uint8_t OST : 1;
        uint8_t RST : 1;
        uint8_t OS : 3;
        uint8_t RAW : 1;
        uint8_t ALT : 1;
    } bit;
    uint8_t reg;
} mpl3115a2_ctrl_reg1;

static bool mpl3115a2_flag_poll(TwoWireWrapper &bus, uint8_t reg, uint8_t flag, uint8_t desired) {
    while (true) {
        uint8_t value;
        if (!I2C_CHECK(bus.read_register_u8(MPL3115A2_I2C_ADDRESS, reg, value))) {
            return false;
        }

        if ((value & flag) == desired) {
            break;
        }

        fk_delay(10);
    }

    return true;
}

static bool mpl3115a2_status_poll(TwoWireWrapper &bus, uint8_t flag) {
    return mpl3115a2_flag_poll(bus, MPL3115A2_REGISTER_STATUS, flag, flag);
}

Mpl3115a2::Mpl3115a2(TwoWireWrapper &bus) : bus_(bus) {
}

Mpl3115a2::~Mpl3115a2() {
}

bool Mpl3115a2::begin() {
    uint8_t identity;
    if (!I2C_CHECK(bus_.read_register_u8(MPL3115A2_I2C_ADDRESS, MPL3115A2_WHOAMI, identity))) {
        return false;
    }

    if (identity != MPL3115A2_WHOAMI_EXPECTED) {
        return false;
    }

    // Initiate a reset, this never returns success. We poll for ready after.
    bus_.write_register_u8(MPL3115A2_I2C_ADDRESS, MPL3115A2_CTRL_REG1, MPL3115A2_CTRL_REG1_RST);
    fk_delay(10); // NOTE Is this too long?

    if (!mpl3115a2_flag_poll(bus_, MPL3115A2_CTRL_REG1, MPL3115A2_CTRL_REG1_RST, 0)) {
        return false;
    }

    // Configure oversampling.
    if (!I2C_CHECK(bus_.write_register_u8(MPL3115A2_I2C_ADDRESS, MPL3115A2_CTRL_REG1, MPL3115A2_CTRL_REG1_OS128))) {
        return false;
    }

    // Configure data ready flags.
    if (!I2C_CHECK(bus_.write_register_u8(MPL3115A2_I2C_ADDRESS, MPL3115A2_PT_DATA_CFG,
                                          MPL3115A2_PT_DATA_CFG_TDEFE | MPL3115A2_PT_DATA_CFG_PDEFE | MPL3115A2_PT_DATA_CFG_DREM))) {
        return false;
    }

    return true;
}

bool Mpl3115a2::get(Mpl3115a2Reading *reading) {
    if (!mpl3115a2_flag_poll(bus_, MPL3115A2_CTRL_REG1, MPL3115A2_CTRL_REG1_OST, 0)) {
        return false;
    }

    // Initiate a reading.
    mpl3115a2_ctrl_reg1 reg1;
    reg1.reg = MPL3115A2_CTRL_REG1_OS128;
    reg1.bit.OST = 1;
    if (!I2C_CHECK(bus_.write_register_u8(MPL3115A2_I2C_ADDRESS, MPL3115A2_CTRL_REG1, reg1.reg))) {
        return false;
    }

    if (!mpl3115a2_status_poll(bus_, MPL3115A2_REGISTER_STATUS_PTDR)) {
        return false;
    }

    uint8_t praw[3];
    if (!I2C_CHECK(bus_.read_register_buffer(MPL3115A2_I2C_ADDRESS, MPL3115A2_REGISTER_PRESSURE_MSB, praw, sizeof(praw)))) {
        return false;
    }

    uint8_t traw[2];
    if (!I2C_CHECK(bus_.read_register_buffer(MPL3115A2_I2C_ADDRESS, MPL3115A2_REGISTER_TEMP_MSB, traw, sizeof(traw)))) {
        return false;
    }

    /**
     * The Pressure data is stored as an unsigned fractional 20-bit value in
     * Pascals in Q18.2 format. The OUT_P_MSB and OUT_P_CSB registers and bits
     * 7-6 of the OUT_P_LSB register contain the integer part in Pascals. Bits
     * 5-4 of OUT_P_LSB con- tain the fractional component. Left shifting the
     * OUT_T_MSB byte by 16 bits into a 32 variable and doing a logical OR with
     * the OUT_T_CSB byte left shifted 8 bits and a logical OR with the
     * OUT_T_LSB byte gives the pressure in Pascals times 64
     */
    uint32_t pressure = ((praw[0] << 16) | (praw[1] << 8) | (praw[2]));

    /**
     * The Temperature data is stored as a signed fractional 12-bit value in °C
     * in Q12.4 format. The OUT_T_MSB register contains the integer part in °C
     * and the OUT_T_LSB register contains the fractional part. Left shifting
     * the OUT_T_MSB byte by 8 bits into a 16-bit variable and adding the
     * OUT_T_LSB byte with a logical OR gives the temperature in °C times 256.
     * When RAW is se- lected then the RAW value is stored in all 16 bits of
     * OUT_T_MSB and OUT_T_LSB.
     */
    uint32_t temperature = ((traw[0] << 8) | (traw[1])) >> 4;
    if (temperature & 0x800) {
        temperature |= 0xF000;
    }

    reading->pressure = pressure / 64.0f / 1000.0f;
    reading->temperature = temperature / 16.0f;

    return true;
}

} // namespace fk