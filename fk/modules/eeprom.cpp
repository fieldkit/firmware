#include "eeprom.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("mod-ee");
/**
 * Return the smaller of two values.
 */
#define MIN(a, b)   ((a > b) ? (b) : (a))

ModuleEeprom::ModuleEeprom(TwoWireWrapper &wire) : wire_(&wire) {
}

static bool read_page(TwoWireWrapper *wire, uint16_t address, uint8_t *data, size_t size) {
    FK_ASSERT(size <= ModuleEeprom::EepromPageSize);

    // TODO This could be done better.
    uint8_t buffer[sizeof(uint16_t)];
    buffer[0] = (address >> 8) & 0xff;
    buffer[1] = (address) & 0xff;

    if (!I2C_CHECK(wire->write(ModuleEeprom::EepromAddress, buffer, sizeof(buffer)))) {
        return false;
    }

    if (!I2C_CHECK(wire->read(ModuleEeprom::EepromAddress, data, size))) {
        return false;
    }

    return true;
}

static bool write_page(TwoWireWrapper *wire, uint16_t address, uint8_t *data, size_t size) {
    FK_ASSERT(size <= ModuleEeprom::EepromPageSize);

    // TODO This could be done better.
    uint8_t buffer[sizeof(uint16_t) + size];
    buffer[0] = (address >> 8) & 0xff;
    buffer[1] = (address) & 0xff;
    memcpy(buffer + sizeof(uint16_t), data, size);

    if (!I2C_CHECK(wire->write(ModuleEeprom::EepromAddress, buffer, sizeof(buffer)))) {
        return false;
    }

    return true;
}

static bool write(TwoWireWrapper *wire, uint16_t address, uint8_t *data, size_t size) {
    uint8_t *ptr = data;
    size_t remaining = size;

    while (remaining > 0) {
        size_t to_write = MIN(EEPROM_PAGE_SIZE, remaining);
        if (!write_page(wire, address, ptr, to_write)) {
            return false;
        }

        ptr += to_write;
        remaining -= to_write;
        address += to_write;
    }

    return true;
}

static bool read(TwoWireWrapper *wire, uint16_t address, uint8_t *data, size_t size) {
    uint8_t *ptr = data;
    size_t remaining = size;

    while (remaining > 0) {
        size_t to_read = MIN(EEPROM_PAGE_SIZE, remaining);
        if (!read_page(wire, address, ptr, to_read)) {
            return false;
        }

        ptr += to_read;
        remaining -= to_read;
        address += to_read;
    }

    return true;
}

bool ModuleEeprom::read_header(ModuleHeader &header) {
    if (!read(wire_, HeaderAddress, (uint8_t *)&header, sizeof(ModuleHeader))) {
        return false;
    }

    return true;
}

bool ModuleEeprom::write_header(ModuleHeader &header) {
    header.crc = fk_module_header_sign(&header);

    if (!write(wire_, HeaderAddress, (uint8_t *)&header, sizeof(ModuleHeader))) {
        logerror("error writing kind");
        return false;
    }

    return true;
}

bool ModuleEeprom::read_data(uint32_t address, void *data, size_t size) {
    if (!read(wire_, address, (uint8_t *)data, size)) {
        return false;
    }

    return true;
}

}
