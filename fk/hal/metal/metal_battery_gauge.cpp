#include "hal/metal/metal_battery_gauge.h"
#include "platform.h"

#include <Wire.h>

namespace fk {

constexpr uint8_t Address = 0x36;
constexpr int32_t RSense = 10;

enum Max17055Registers {
    STATUS_REG                 = 0x00, /*!< 0x00 default value = 0x0002 */
    VALRTTH_REG                = 0x01, /*!< 0x01 */
    TALRTTH_REG                = 0x02, /*!< 0x02 */
    SALRTTH_REG                = 0x03, /*!< 0x03 */
    ATRATE_REG                 = 0x04, /*!< 0x04 write negative 2s comp of a 16-bit theoretical load */
    REPCAP_REG                 = 0x05, /*!< 0x05 */
    REPSOC_REG                 = 0x06, /*!< 0x06 */
    TEMP_REG                   = 0x08, /*!< 0x08 */
    VCELL_REG                  = 0x09, /*!< 0x09 */
    CURRENT_REG                = 0x0A, /*!< 0x0A */
    AVGCURRENT_REG             = 0x0B, /*!< 0x0B */
    MIXSOC_REG                 = 0x0D, /*!< 0x0D */
    AVSOC_REG                  = 0x0E, /*!< 0x0E */
    MIXCAP_REG                 = 0x0F, /*!< 0x0F */

    FULLCAPREP_REG             = 0x10, /*!< 0x10 */
    TTE_REG                    = 0X11, /*!< 0x11 */
    QRTABLE00_REG              = 0x12, /*!< 0x12 */
    FULLSOCTHR_REG             = 0x13, /*!< 0x13 */
    CYCLES_REG                 = 0x17, /*!< 0x17 */
    DESIGNCAP_REG              = 0x18, /*!< 0x18 */
    AVGVCELL_REG               = 0x19, /*!< 0x19 */
    MAXMINVOLT_REG             = 0x1B, /*!< 0x1B */
    CONFIG_REG                 = 0x1D, /*!< 0x1D default = 0x2210 */
    ICHGTERM_REG               = 0x1E, /*!< 0x1E */

    TTF_REG                    = 0x20, /*!< 0x20 */
    VERSION_REG                = 0x21, /*!< 0x21 */
    QRTABLE10_REG              = 0x22, /*!< 0x22 */
    FULLCAPNOM_REG             = 0x23, /*!< 0x23 */
    LEARNCFG_REG               = 0x28, /*!< 0x28 */
    RELAXCFG_REG               = 0x2A, /*!< 0x2A */
    TGAIN_REG                  = 0x2C, /*!< 0x2C */
    TOFF_REG                   = 0x2D, /*!< 0x2D */

    QRTABLE20_REG              = 0x32, /*!< 0x32 */
    RCOMP0_REG                 = 0x38, /*!< 0x38 */
    TEMPCO_REG                 = 0x39, /*!< 0x39 */
    VEMPTY_REG                 = 0x3A, /*!< 0x39 */
    FSTAT_REG                  = 0x3D, /*!< 0x39 */

    QRTABLE30_REG              = 0x42, /*!< 0x39 */
    DQACC_REG                  = 0x45, /*!< 0x39 */
    DPACC_REG                  = 0x46, /*!< 0x39 */
    VFSOC0_REG                 = 0x48, /*!< 0x39 */
    QH0_REG                    = 0x4C, /*!< 0x39 */
    QH_REG                     = 0x4D, /*!< 0x39 */

    VFSOC0_QH0_LOCK_REG        = 0x60, /*!< 0x39 */
    LOCK1_REG                  = 0x62, /*!< 0x39 */
    LOCK2_REG                  = 0x63, /*!< 0x39 */

    MODELDATA_START_REG        = 0x80, /*!< 0x39 */

    IALRTTH_REG                = 0xB4, /*!< 0x39 */
    CURVE_REG                  = 0xB9, /*!< 0x39 */
    HIBCFG_REG                 = 0xBA, /*!< 0x39 default = 0x870C (0x890C)*/
    CONFIG2_REG                = 0xBB, /*!< 0xBB default = 0x3658 */

    MODELCFG_REG               = 0xDB, /*!< 0xDB */
    ATTTE_REG                  = 0xDD, /*!< 0xDD */
    ATAVSOC_REG                = 0xDE, /*!< 0xDE */
    ATAVCAP_REG                = 0xDF, /*!< 0xDF */

    OCV_REG                    = 0xFB, /*!< 0x39 */
    VFSOC_REG                  = 0xFF  /*!< 0x39 */
};

#define MAX17055_STATUS_BST             (1 << 3)
#define MAX17055_STATUS_POR             (1 << 1)
#define MAX17055_POR_MASK               (0xFFFD)
#define MAX17055_MODELCFG_REFRESH       (1 << 15)
#define MAX17055_FSTAT_DNR              (1)

FK_DECLARE_LOGGER("battery");

static bool read_u16(uint8_t address, uint8_t reg, uint16_t &value) {
    uint8_t bytes[2];
    Wire1.beginTransmission(address);
    Wire1.write(reg);
    if (Wire1.endTransmission() != 0) {
        return false;
    }

    Wire1.requestFrom(address, 2);
    bytes[0] = Wire1.read();
    bytes[1] = Wire1.read();
    value = ((bytes[1] & 0x00ff) << 8) + bytes[0];
    return true;
}

static bool write_u16(uint8_t address, uint8_t reg, uint16_t value) {
    Wire1.beginTransmission(address);
    Wire1.write(reg);
    Wire1.write( value       & 0xFF);
    Wire1.write((value >> 8) & 0xFF);
    return Wire1.endTransmission() == 0;
}

static bool write_u16_and_verify(uint8_t address, uint8_t reg, uint16_t value) {
    uint8_t tries = 10;
    uint16_t data = 0;

    do {
        if (!write_u16(address, reg, value)) {
            // status = false;
        }

        fk_delay(3);

        if (!read_u16(address, reg, data)) {
            // status = false;
        }

        tries--;
    }
    while (tries > 0 && data != value);

    return data == value;
}

bool check_por() {
    uint16_t status;
    if (!read_u16(Address, Max17055Registers::STATUS_REG, status)) {
        return false;
    }
    return status & MAX17055_STATUS_POR;
}

bool clear_por() {
    uint16_t status;
    if (!read_u16(Address, Max17055Registers::STATUS_REG, status)) {
        return false;
    }

    if (!write_u16_and_verify(Address, Max17055Registers::STATUS_REG, status & MAX17055_POR_MASK)) {
        return false;
    }

    return true;
}

bool poll_until_cleared(Max17055Registers reg, uint16_t mask, uint16_t to) {
    do {
        fk_delay(1);

        uint16_t value;
        if (!read_u16(Address, reg, value)) {
            return false;
        }

        if (!(value & mask)) {
            return true;
        }

        to--;
    }
    while (to > 0);

    return false;
}

int32_t get_temperature() {
    uint16_t temp;

    if (!read_u16(Address, Max17055Registers::TEMP_REG, temp)) {
        return false;
    }

    // Check for signed value.
    if (temp & 0x8000) {
        temp |= 0xffff0000;
    }

    // LSB = 1/256 deg C.
    temp >>= 8;

    return temp;
}

int32_t get_state_of_charge() {
    uint16_t value;

    if (!read_u16(Address, Max17055Registers::REPSOC_REG, value)) {
        return false;
    }

    // LSB = 1/256%
    value >>= 8;

    return value;
}

float get_time_to_empty() {
    uint16_t value;

    if (!read_u16(Address, Max17055Registers::TTE_REG, value)) {
        return false;
    }

    // LSB = 1/5.625s

    return ((float)value) * 5.625;
}

uint16_t wakeup() {
    uint16_t hibernate_config;

    read_u16(Address, Max17055Registers::HIBCFG_REG, hibernate_config);

    write_u16(Address, Max17055Registers::VFSOC0_QH0_LOCK_REG, 0x90);
    write_u16(Address, Max17055Registers::HIBCFG_REG, 0x0);
    write_u16(Address, Max17055Registers::VFSOC0_QH0_LOCK_REG, 0x0);

    return hibernate_config;
}

static int32_t lsb_to_uvolts(uint16_t lsb) {
    return (lsb * 625) / 8; /* 78.125uV per bit */
}

int32_t get_cell_voltage() {
    uint16_t value;

    if (!read_u16(Address, Max17055Registers::VCELL_REG, value)) {
        return false;
    }

    return lsb_to_uvolts(value);
}

static int32_t raw_current_to_uamps(uint32_t curr, int32_t rsense_value) {
    int32_t res = curr;
    /* Negative? */
    if (res & 0x8000) {
        res |= 0xFFFF0000;
    }
    else {
        res *= 1562500 / (rsense_value * 1000);
    }
    return res;
}

int32_t get_current() {
    uint16_t value;

    if (!read_u16(Address, Max17055Registers::CURRENT_REG, value)) {
        return false;
    }

    return raw_current_to_uamps(value, RSense);
}

MetalBatteryGauge::MetalBatteryGauge() {
}

constexpr uint16_t MAX17055_DEVNAME = 0x4010;

bool MetalBatteryGauge::begin() {
    uint16_t version;

    if (!read_u16(Address, Max17055Registers::VERSION_REG, version)) {
        return false;
    }

    if (version != MAX17055_DEVNAME) {
        logerror("invalid version (%x)", version);
        return false;
    }

    if (!check_por()) {
        logerror("no por");
        return false;
    }

    if (!poll_until_cleared(Max17055Registers::FSTAT_REG, MAX17055_FSTAT_DNR, 800)) {
        logerror("dnr never cleared");
        return false;
    }

    auto hibernate_config = wakeup();

    // Initialize Model

    if (!write_u16(Address, Max17055Registers::HIBCFG_REG, hibernate_config)) {
        return false;
    }

    if (!clear_por()) {
        return false;
    }

    available_ = true;

    return true;
}

BatteryReading MetalBatteryGauge::get() {
    if (!available_) {
        return {
            .available = false,
            .cellv = 0,
            .soc = 0,
            .temp = 0,
            .tte = 0.0f,
            .ttf = 0.0f,
        };
    }

    auto cellv = get_cell_voltage();
    auto soc = get_state_of_charge();
    auto tte = get_time_to_empty();
    auto temp = get_temperature();

    return {
        .available = true,
        .cellv = cellv,
        .soc = soc,
        .temp = temp,
        .tte = tte,
        .ttf = 0.0f,
    };
}


}
