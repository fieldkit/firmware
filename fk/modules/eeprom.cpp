#include "eeprom.h"
#include "platform.h"
#include "protobuf.h"

namespace fk {

FK_DECLARE_LOGGER("mod-ee");
/**
 * Return the smaller of two values.
 */
#define MIN(a, b) ((a > b) ? (b) : (a))

ModuleEeprom::ModuleEeprom(TwoWireWrapper &wire) : wire_(&wire) {
}

static bool read_page(TwoWireWrapper *wire, uint16_t address, uint8_t *data, size_t size) {
    FK_ASSERT(size <= ModuleEeprom::EepromPageSize);
    FK_ASSERT(address + size <= ModuleEeprom::EepromSize);

    // TODO This could be done better.
    uint8_t buffer[sizeof(uint16_t)];
    buffer[0] = (address >> 8) & 0xff;
    buffer[1] = (address)&0xff;

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
    FK_ASSERT(address + size <= ModuleEeprom::EepromSize);

    // TODO This could be done better.
    uint8_t buffer[sizeof(uint16_t) + size];
    buffer[0] = (address >> 8) & 0xff;
    buffer[1] = (address)&0xff;
    memcpy(buffer + sizeof(uint16_t), data, size);

    if (!I2C_CHECK(wire->write(ModuleEeprom::EepromAddress, buffer, sizeof(buffer)))) {
        return false;
    }

    auto to = EEPROM_TIMEOUT_WRITE;
    while (to > 0) {
        if (I2C_CHECK(wire->read(ModuleEeprom::EepromAddress, nullptr, 0))) {
            return true;
        }

        fk_delay(1);
        to--;
    }

    return false;
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
        logerror("error writing header");
        return false;
    }

    return true;
}

bool ModuleEeprom::read_configuration(uint8_t **buffer, size_t &size, Pool *pool) {
    if (!read_data_delimited(ConfigurationAddress, buffer, size, pool)) {
        return false;
    }

    loginfo("configuration size=%zd", size);

    return true;
}

static bool read_callback(pb_istream_t *stream, uint8_t *buf, size_t c) {
    return reinterpret_cast<ModuleEeprom *>(stream->state)->read_stream(buf, c);
}

pb_istream_t pb_istream_from_eeprom(ModuleEeprom *eeprom) {
    return { &read_callback, (void *)eeprom, UINT32_MAX, 0 };
}

bool ModuleEeprom::read_configuration(void *record, pb_msgdesc_t const *fields) {
    stream_position_ = ConfigurationAddress;
    pb_istream_t istream = pb_istream_from_eeprom(this);
    if (!pb_decode_delimited(&istream, fields, record)) {
        return 0;
    }

    return true;
}

bool ModuleEeprom::write_configuration(uint8_t const *buffer, size_t size) {
    if (!write(wire_, ConfigurationAddress, (uint8_t *)buffer, size)) {
        return false;
    }

    return true;
}

bool ModuleEeprom::erase_configuration() {
    if (!erase_page(ConfigurationAddress)) {
        return false;
    }

    loginfo("configuration erased");

    return true;
}

bool ModuleEeprom::read_stream(void *data, size_t size) {
    if (!read(wire_, stream_position_, (uint8_t *)data, size)) {
        return false;
    }

    stream_position_ += size;

    return true;
}

bool ModuleEeprom::read_data(uint32_t address, void *data, size_t size) {
    if (!read(wire_, address, (uint8_t *)data, size)) {
        return false;
    }

    return true;
}

bool ModuleEeprom::erase_all() {
    for (auto address = 0u; address < EepromSize; address += EepromPageSize) {
        if (!erase_page(address)) {
            logerror("error erasing (0x%0" PRIx32 ")", (uint32_t)address);
            return false;
        }
    }

    return true;
}

bool ModuleEeprom::erase_page(uint32_t address) {
    uint8_t page[EepromPageSize];
    memset(page, 0xff, sizeof(page));
    if (!write(wire_, address, (uint8_t *)page, sizeof(page))) {
        logerror("error erasing (0x%0" PRIx32 ")", (uint32_t)address);
        return false;
    }
    return true;
}

bool ModuleEeprom::read_data_delimited(uint32_t address, uint8_t **buffer, size_t &bytes_read, Pool *pool) {
    *buffer = nullptr;
    bytes_read = 0;

    pb_byte_t size_bytes_and_some[8];
    if (!read_page(wire_, address, size_bytes_and_some, sizeof(size_bytes_and_some))) {
        return false;
    }

    uint32_t encoded_size = 0u;
    auto stream = pb_istream_from_buffer((pb_byte_t *)size_bytes_and_some, sizeof(size_bytes_and_some));
    if (!::pb_decode_varint32(&stream, &encoded_size)) {
        return true;
    }

    auto buffer_size = pb_varint_size(encoded_size) + encoded_size;
    loginfo("eeprom: allocating %" PRIu32 " bytes", buffer_size);

    auto ptr = (uint8_t *)pool->malloc(buffer_size);
    auto remaining = buffer_size;

    while (remaining > 0) {
        size_t to_read = MIN(EEPROM_PAGE_SIZE, remaining);
        if (!read_page(wire_, address, ptr, to_read)) {
            return false;
        }

        ptr += to_read;
        remaining -= to_read;
        address += to_read;
    }

    *buffer = ptr;
    bytes_read = buffer_size;

    return true;
}

} // namespace fk
