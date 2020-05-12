#include "temperature.h"

namespace fk {

#if defined(linux)

CoreTemperature::CoreTemperature(TwoWireWrapper wire) : wire_(wire) {
}

bool CoreTemperature::begin() {
    return false;
}

bool CoreTemperature::read(float *temperature) {
    return false;
}

#else

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

    auto temp_data = (int16_t)((lsb << 8) | msb) >> 5;
    auto mask = (1 << 10);
    if ((temp_data & mask) == mask) {
        auto temp = temp_data * 0.125f;
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

#endif

}
