#include "eeprom.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("mod-ee");

ModuleEeprom::ModuleEeprom(TwoWireWrapper &wire) : wire_(&wire) {
}

template<typename T>
static bool read(TwoWireWrapper *wire, uint16_t address, T &data) {
    static_assert(sizeof(T) <= ModuleEeprom::EepromPageSize, "sizeof(T) <= EepromPageSize");

    if (!I2C_CHECK(wire->write(ModuleEeprom::EepromAddress, &address, sizeof(uint16_t)))) {
        return false;
    }

    if (!I2C_CHECK(wire->read(ModuleEeprom::EepromAddress, &data, sizeof(T)))) {
        return false;
    }

    return true;
}

template<typename T>
static bool write(TwoWireWrapper *wire, uint16_t address, T &data) {
    static_assert(sizeof(T) <= ModuleEeprom::EepromPageSize, "sizeof(T) <= EepromPageSize");

    uint8_t buffer[sizeof(uint16_t) + sizeof(T)];
    buffer[0] = (address >> 8) & 0xff;
    buffer[1] = (address);
    memcpy(buffer + sizeof(uint16_t), &data, sizeof(T));

    if (!I2C_CHECK(wire->write(ModuleEeprom::EepromAddress, buffer, sizeof(buffer)))) {
        return false;
    }

    return true;
}

bool ModuleEeprom::read_header(ModuleHeader &header) {
    ModuleKind kind;
    if (!read_kind(kind)) {
        return false;
    }

    if (!fk_module_kind_valid(&kind)) {
        logerror("invalid kind");
        return false;
    }

    ModuleIdentity identity;
    if (!read_identity(identity)) {
        return false;
    }

    if (!fk_module_identity_valid(&identity)) {
        logerror("invalid identity");
        return false;
    }

    header.manufacturer = kind.manufacturer;
    header.kind = kind.kind;
    header.version = kind.version;
    header.id = identity.id;

    return true;
}

bool ModuleEeprom::write_header(ModuleHeader &header) {
    ModuleKind kind = {
        .manufacturer = header.manufacturer,
        .kind = header.kind,
        .version = header.version,
        .reserved = { 0, 0, 0, 0 },
        .crc = 0x00,
    };

    kind.crc = fk_module_kind_sign(&kind);

    if (!write_kind(kind)) {
        logerror("error writing kind");
        return false;
    }

    fk_delay(100);

    ModuleIdentity identity = {
        .id = header.id,
        .reserved = { 0, 0, 0 },
        .crc = 0,
    };

    identity.crc = fk_module_identity_sign(&identity);

    if (!write_identity(identity)) {
        logerror("error writing identity");
        return false;
    }

    fk_delay(100);

    return true;
}

bool ModuleEeprom::read_kind(ModuleKind &kind) {
    return read<ModuleKind>(wire_, HeaderAddress, kind);
}

bool ModuleEeprom::write_kind(ModuleKind &kind) {
    return write<ModuleKind>(wire_, HeaderAddress, kind);
}

bool ModuleEeprom::read_identity(ModuleIdentity &identity) {
    return read<ModuleIdentity>(wire_, HeaderAddress + EepromPageSize, identity);
}

bool ModuleEeprom::write_identity(ModuleIdentity &identity) {
    return write<ModuleIdentity>(wire_, HeaderAddress + EepromPageSize, identity);
}

}
