#include "eeprom.h"

namespace fk {

ModuleEeprom::ModuleEeprom(TwoWireWrapper &wire) : wire_(&wire) {
}

bool ModuleEeprom::read_header(ModuleHeader &header) {
    static_assert(sizeof(ModuleHeader) == EepromPageSize, "ModuleHeader should be same size as one EEPROM page");

    auto address = HeaderAddress;
    if (!I2C_CHECK(wire_->write(EepromAddress, &address, sizeof(address)))) {
        return false;
    }

    if (!I2C_CHECK(wire_->read(EepromAddress, &header, sizeof(ModuleHeader)))) {
        return false;
    }

    return true;
}

bool ModuleEeprom::write_header(ModuleHeader &header) {
    static_assert(sizeof(ModuleHeader) == EepromPageSize, "ModuleHeader should be same size as one EEPROM page");

    auto address = HeaderAddress;
    uint8_t buffer[sizeof(HeaderAddress) + sizeof(ModuleHeader)];
    memcpy(buffer,                         &address, sizeof(HeaderAddress));
    memcpy(buffer + sizeof(HeaderAddress), &header, sizeof(ModuleHeader));

    if (!I2C_CHECK(wire_->write(EepromAddress, buffer, sizeof(buffer)))) {
        return false;
    }

    return true;
}

}
