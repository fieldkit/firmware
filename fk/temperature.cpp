#include "temperature.h"

namespace fk {

CoreTemperature::CoreTemperature(TwoWireWrapper wire) : wire_(wire) {
}

bool CoreTemperature::begin() {
    return read(nullptr);
}

bool CoreTemperature::read(float *temperature) {
    uint8_t buffer[2];

    if (!I2C_CHECK(wire_.read_register_buffer(Address, 0x00, buffer, sizeof(buffer)))) {
        return false;
    }

    auto lsb = buffer[0];
    auto msb = buffer[1];

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
