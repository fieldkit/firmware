#include "modules.h"
#include "crc.h"

uint32_t fk_module_header_sign(ModuleHeader const *header) {
    return crc32_checksum(FK_MODULES_CRC_SEED, (uint8_t *)header, sizeof(ModuleHeader) - sizeof(uint32_t));
}

uint32_t fk_module_header_valid(ModuleHeader const *header) {
    return header->crc == fk_module_header_sign(header);
}
