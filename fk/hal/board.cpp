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

    rv = read(address, &value, sizeof(reg));
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

EepromLock::~EepromLock() {
    get_board()->release_eeprom();
}

}
