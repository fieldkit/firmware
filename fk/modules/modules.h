#pragma once

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Header that's stored in every Module EEPROM that informs the core firmware
 * the kind of module that they're dealing with. This should never exceed 32 bytes.
 */
typedef struct ModuleHeader {
    uint32_t manufacturer;
    uint32_t kind;
    uint32_t version;
    uint32_t reserved[4];
    uint32_t crc;
} ModuleHeader;

uint32_t fk_module_header_sign(ModuleHeader *header);

uint32_t fk_module_header_valid(ModuleHeader *header);

#define FK_MODULES_MANUFACTURER                    (0x01)
#define FK_MODULES_KIND_WEATHER                    (0x01)
#define FK_MODULES_KIND_WATER                      (0x02)
#define FK_MODULES_KIND_RANDOM                     (0xa1)

#ifdef __cplusplus
}
#endif
