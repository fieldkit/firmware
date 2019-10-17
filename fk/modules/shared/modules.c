#include "modules.h"
#include "crc.h"

uint32_t fk_module_header_sign(ModuleHeader const *header) {
    return crc32_checksum(FK_MODULES_CRC_SEED, (uint8_t *)header, sizeof(ModuleHeader) - sizeof(uint32_t));
}

uint32_t fk_module_header_valid(ModuleHeader const *header) {
    return header->crc == fk_module_header_sign(header);
}

uint8_t fk_module_is_available_i2c_address(uint8_t address) {
    switch (address) {
    case 0x20: return 0;
    case 0x4d: return 0;
    case 0x50: return 0;
    case 0x58: return 0;
    case 0x70: return 0;
    default: return 1;
    }
}
