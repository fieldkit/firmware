#include "ads_1219.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("ads1219");

#define ADS1219_CONFIG_REGISTER_ADDRESS 0x40
#define ADS1219_STATUS_REGISTER_ADDRESS 0x24

#define ADS1219_MUX_MASK 				0x1F
#define ADS1219_MUX_DIFF_0_1			0x00
#define ADS1219_MUX_DIFF_2_3			0x20
#define ADS1219_MUX_DIFF_1_2			0x40
#define ADS1219_MUX_SINGLE_0			0x60
#define ADS1219_MUX_SINGLE_1			0x80
#define ADS1219_MUX_SINGLE_2			0xA0
#define ADS1219_MUX_SINGLE_3			0xC0
#define ADS1219_MUX_SHORTED				0xE0

#define ADS1219_GAIN_MASK 				0xEF
#define ADS1219_GAIN_ONE				0x00
#define ADS1219_GAIN_FOUR				0x10

#define ADS1219_DATA_RATE_MASK			0xF3
#define ADS1219_DATA_RATE_20			0x00
#define ADS1219_DATA_RATE_90			0x04
#define ADS1219_DATA_RATE_330			0x08
#define ADS1219_DATA_RATE_1000			0x0c

#define ADS1219_MODE_MASK				0xFD
#define ADS1219_MODE_SINGLE_SHOT		0x00
#define ADS1219_MODE_CONTINUOUS			0x02

Ads1219::Ads1219(TwoWireWrapper &bus, uint8_t address, Ads1219ReadyChecker *ready) : bus_(bus), address_(address), ready_(ready), config_(0) {
}

Ads1219::~Ads1219()  {
}

bool Ads1219::begin() {
    return true;
}

bool Ads1219::set_voltage_reference(Ads1219VoltageReference vref) {
    config_ |= (uint8_t)vref;
    return I2C_CHECK(bus_.write_register_u8(address_, ADS1219_CONFIG_REGISTER_ADDRESS, config_));
}

bool Ads1219::read_single_ended(int32_t channel, int32_t &value) {
    switch (channel) {
    case (0):
        config_ |= ADS1219_MUX_SINGLE_0;
        break;
    case (1):
        config_ |= ADS1219_MUX_SINGLE_1;
        break;
    case (2):
        config_ |= ADS1219_MUX_SINGLE_2;
        break;
    case (3):
        config_ |= ADS1219_MUX_SINGLE_3;
        break;
    default:
        break;
    }

    if (!I2C_CHECK(bus_.write_register_u8(address_, ADS1219_CONFIG_REGISTER_ADDRESS, config_))) {
        return false;
    }

    if (!start()) {
        return false;
    }

    if (!ready_->block_until_ready(bus_)) {
        return false;
    }

    return read_conversion(value);
}

bool Ads1219::read_differential_0_1(int32_t &value) {
    return read_dfferential(ADS1219_MUX_DIFF_0_1, value);
}

bool Ads1219::read_differential_2_3(int32_t &value) {
    return read_dfferential(ADS1219_MUX_DIFF_2_3, value);
}

bool Ads1219::read_dfferential_1_2(int32_t &value) {
    return read_dfferential(ADS1219_MUX_DIFF_1_2, value);
}

bool Ads1219::read_dfferential(uint8_t config, int32_t &value) {
    config_ |= config;

    logdebug("config: %x", config_);

    if (!I2C_CHECK(bus_.write_register_u8(address_, ADS1219_CONFIG_REGISTER_ADDRESS, config_))) {
        return false;
    }

    if (!start()) {
        return false;
    }

    if (!ready_->block_until_ready(bus_)) {
        return false;
    }

    return read_conversion(value);
}

bool Ads1219::read_conversion(int32_t &value) {
    uint8_t data[3] = { 0xff, 0xff, 0xff };

    if (!I2C_CHECK(bus_.read_register_buffer(address_, 0x10, data, sizeof(data)))) {
        return false;
    }

    value  = data[0];
    value <<= 8;
    value |= data[1];
    value <<= 8;
    value |= data[2];
    value <<= 8;
    value >>= 8;

    logdebug("0x%x 0x%x 0x%x = 0x%x", data[0], data[1], data[2], value);

    return true;
}

bool Ads1219::start() {
    return I2C_CHECK(bus_.write_u8(address_, 0x08));
}

}
