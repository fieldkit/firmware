#include "ads_1219.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("ads1219");

#define ADS1219_CONFIG_REGISTER_ADDRESS 0x40
#define ADS1219_STATUS_REGISTER_ADDRESS 0x24

Ads1219::Ads1219(TwoWireWrapper &bus, uint8_t address, Ads1219ReadyChecker *ready) : bus_(bus), address_(address), ready_(ready), config_(0) {
}

Ads1219::~Ads1219()  {
}

bool Ads1219::begin() {
    return true;
}

bool Ads1219::configure(Ads1219VoltageReference vref, Ads1219Channel channel) {
    config_ = (uint8_t)vref | (uint8_t)channel;
    return I2C_CHECK(bus_.write_register_u8(address_, ADS1219_CONFIG_REGISTER_ADDRESS, config_));
}

bool Ads1219::read(int32_t &value) {
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
