#include "ads_1219.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("ads1219");

#define ADS1219_COMMAND_RESET      0x06
#define ADS1219_COMMAND_START      0x08
#define ADS1219_COMMAND_POWER_DOWN 0x02
#define ADS1219_COMMAND_READ_DATA  0x10

#define ADS1219_COMMAND_WRITE_CONFIG_REGISTER 0x40
#define ADS1219_COMMAND_READ_STATUS_REGISTER  0x24
#define ADS1219_COMMAND_READ_CONFIG_REGISTER  0x20

Ads1219::Ads1219(TwoWireWrapper &bus, uint8_t address, Ads1219ReadyChecker *ready)
    : bus_(bus), address_(address), ready_(ready), config_(0) {
}

Ads1219::~Ads1219() {
}

bool Ads1219::begin() {
    if (!I2C_CHECK(bus_.write_u8(address_, ADS1219_COMMAND_RESET))) {
        return false;
    }

    return true;
}

bool Ads1219::configure(Ads1219VoltageReference vref, Ads1219Channel channel, Ads1219Gain gain, Ads1219DataRate dr) {
    config_ = (uint8_t)vref | (uint8_t)channel | (uint8_t)gain | (uint8_t)dr;

    logdebug("config=0x%x", config_);
    if (!I2C_CHECK(bus_.write_register_u8(address_, ADS1219_COMMAND_WRITE_CONFIG_REGISTER, config_))) {
        return false;
    }

    uint8_t config_after = 0;
    if (!I2C_CHECK(bus_.read_register_u8(address_, ADS1219_COMMAND_READ_CONFIG_REGISTER, config_after))) {
        return false;
    }

    logdebug("config-after=0x%x", config_after);

    uint8_t status_after = 0;
    if (!I2C_CHECK(bus_.read_register_u8(address_, ADS1219_COMMAND_READ_STATUS_REGISTER, status_after))) {
        return false;
    }

    logdebug("status-after=0x%x", status_after);

    return true;
}

bool Ads1219::read(int32_t &value) {
    if (!start()) {
        return false;
    }

    if (!ready_->block_until_ready(bus_)) {
        return false;
    }

    /*
    uint8_t status_after = 0;
    if (!I2C_CHECK(bus_.read_register_u8(address_, ADS1219_COMMAND_READ_STATUS_REGISTER, status_after))) {
        return false;
    }

    logdebug("status-before-read=0x%x", status_after);
    */

    if (!read_conversion(value)) {
        return false;
    }

    return true;
}

bool Ads1219::read_conversion(int32_t &value) {
    uint8_t data[3] = { 0xff, 0xff, 0xff };

    if (!I2C_CHECK(bus_.read_register_buffer(address_, ADS1219_COMMAND_READ_DATA, data, sizeof(data)))) {
        return false;
    }

    value = ((int32_t)data[0]) & 0xff;
    value <<= 8;
    value |= ((int32_t)data[1]) & 0xff;
    value <<= 8;
    value |= ((int32_t)data[2]) & 0xff;
    value <<= 8;
    value >>= 8;

    logdebug("0x%x 0x%x 0x%x = 0x%x", data[0], data[1], data[2], value);

    /*
    uint8_t status_after = 0;
    if (!I2C_CHECK(bus_.read_register_u8(address_, ADS1219_COMMAND_READ_STATUS_REGISTER, status_after))) {
        return false;
    }

    logdebug("status-after=0x%x", status_after);
    */

    return true;
}

bool Ads1219::start() {
    return I2C_CHECK(bus_.write_u8(address_, ADS1219_COMMAND_START));
}

} // namespace fk
