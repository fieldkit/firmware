#pragma once

#include "common.h"
#include "modules/shared/modules.h"
#include "hal/board.h"

namespace fk {

class ModuleEeprom {
public:
    constexpr static uint8_t EepromAddress = EEPROM_I2C_ADDRESS;
    constexpr static size_t EepromPageSize = EEPROM_PAGE_SIZE;
    constexpr static size_t EepromSize = EEPROM_ADDRESS_END;
    constexpr static uint16_t HeaderAddress = EEPROM_ADDRESS_HEADER;
    constexpr static uint16_t ConfigurationAddress = EEPROM_ADDRESS_CONFIG;

private:
    TwoWireWrapper *wire_;

public:
    explicit ModuleEeprom(TwoWireWrapper &wire);

public:
    bool read_header(ModuleHeader &header);
    bool write_header(ModuleHeader &header);
    bool read_configuration(uint8_t *buffer, size_t &size, size_t buffer_size);
    bool write_configuration(uint8_t const *buffer, size_t size);
    bool erase_configuration();
    bool erase_all();

public:
    bool read_data(uint32_t address, void *data, size_t size);
    bool read_data_delimited(uint32_t address, uint8_t *buffer, size_t &bytes_read, size_t buffer_size);
    bool erase_page(uint32_t address);

};

} // namespace fk
