#include "board.h"

namespace fk {

static Board board;

Board *get_board() {
    return &board;
}

int32_t TwoWireWrapper::read_register_u8(uint8_t address, uint8_t reg, uint8_t &value) {
    int32_t rv;

    rv = write(address, &reg, sizeof(reg));
    if (!I2C_CHECK(rv)) {
        return rv;
    }

    rv = read(address, &value, sizeof(value));
    if (!I2C_CHECK(rv)) {
        return rv;
    }

    return 0;
}

int32_t TwoWireWrapper::write_register_u8(uint8_t address, uint8_t reg, uint8_t value) {
    uint8_t command[] = {
        reg,
        value
    };
    return write(address, &command, sizeof(command));
}

int32_t TwoWireWrapper::write_u8(uint8_t address, uint8_t value) {
    uint8_t command[] = {
        value
    };
    return write(address, &command, sizeof(command));
}

int32_t TwoWireWrapper::read_register_buffer(uint8_t address, uint8_t reg, uint8_t *buffer, int32_t size) {
    int32_t rv;

    rv = write(address, &reg, sizeof(reg));
    if (!I2C_CHECK(rv)) {
        return rv;
    }

    rv = read(address, buffer, size);
    if (!I2C_CHECK(rv)) {
        return rv;
    }

    return 0;
}

int32_t TwoWireWrapper::write_register_u16(uint8_t address, uint8_t reg, uint16_t value) {
    uint8_t command[] = {
        reg,
        (uint8_t)((value     ) & 0xff),
        (uint8_t)((value >> 8) & 0xff),
    };
    return write(address, &command, sizeof(command));
}

int32_t TwoWireWrapper::read_register_u16(uint8_t address, uint8_t reg, uint16_t &value) {
    auto rv = write(address, &reg, sizeof(reg));
    if (!I2C_CHECK(rv)) {
        return rv;
    }

    uint8_t bytes[2];
    rv = read(address, &bytes, sizeof(bytes));
    if (!I2C_CHECK(rv)) {
        return rv;
    }

    value = ((bytes[1] & 0xff) << 8) + bytes[0];

    return 0;
}

class AcquireBusI2cRadio : public AcquireTwoWireBus {
public:
    TwoWireWrapper acquire() override {
        return get_board()->i2c_radio();
    }
};

class AcquireBusI2cModule : public AcquireTwoWireBus {
public:
    TwoWireWrapper acquire() override {
        return get_board()->i2c_module();
    }
};

AcquireBusI2cModule acquire_i2c_radio_;

AcquireTwoWireBus *Board::acquire_i2c_radio() {
    return &acquire_i2c_radio_;
}

AcquireBusI2cModule acquire_i2c_module_;

AcquireTwoWireBus *Board::acquire_i2c_module() {
    return &acquire_i2c_module_;
}

EepromLock::~EepromLock() {
    get_board()->release_eeprom();
}

}
