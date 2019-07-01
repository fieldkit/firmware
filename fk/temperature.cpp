#include "temperature.h"

namespace fk {

CoreTemperature::CoreTemperature(TwoWire &wire) : wire_(&wire) {
}

bool CoreTemperature::begin() {
    return read(nullptr);
}

bool CoreTemperature::read(float *temperature) {
    wire_->beginTransmission(Address);
    wire_->write(0x00);
    wire_->requestFrom(0x48, 2);
    auto lsb = wire_->read();
    auto msb = wire_->read();
    if (!I2C_CHECK(wire_->endTransmission())) {
        return false;
    }

    auto temp_data = ((lsb << 8) | msb) >> 5;

    if ((temp_data & (1 << 10)) == 1) {
        auto temp = (int16_t)temp_data * 0.125f;
        if (temperature != nullptr) {
            *temperature = temp;
        }
    }
    else {
        auto temp = temp_data * 0.125f;
        if (temperature != nullptr) {
            *temperature = temp;
        }
    }
    return true;
}

}
