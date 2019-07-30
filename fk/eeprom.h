#pragma once

#include "common.h"
#include "hal/board.h"
#include "modules/modules.h"

namespace fk {

class ModuleEeprom {
private:
    constexpr static uint8_t EepromAddress = 0x50;
    constexpr static size_t EepromPageSize = 32;
    constexpr static uint16_t HeaderAddress = 0x00;

private:
    TwoWireWrapper *wire_;

public:
    ModuleEeprom(TwoWireWrapper &wire);

public:
    bool read_header(ModuleHeader &header);

    bool write_header(ModuleHeader &header);

};

}
