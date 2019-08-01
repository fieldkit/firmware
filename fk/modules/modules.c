#include "modules.h"
#include "crc.h"

uint32_t fk_module_header_sign(ModuleHeader *header) {
    return crc32_checksum((uint8_t *)header, sizeof(ModuleHeader) - sizeof(uint32_t));
}

uint32_t fk_module_header_valid(ModuleHeader *header) {
    return header->crc == fk_module_header_sign(header);
}
