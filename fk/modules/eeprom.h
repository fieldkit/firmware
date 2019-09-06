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

private:
    TwoWireWrapper *wire_;

public:
    ModuleEeprom(TwoWireWrapper &wire);

public:
    bool read_header(ModuleHeader &header);
    bool write_header(ModuleHeader &header);
    bool read_data(uint32_t address, void *data, size_t size);

};

}
