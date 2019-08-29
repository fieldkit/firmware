#pragma once

#include <inttypes.h>

#include "uuid.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ModuleHeader {
    uint32_t manufacturer;
    uint32_t kind;
    uint32_t version;
    fk_uuid_t id;
} ModuleHeader;

typedef struct ModuleKind {
    uint32_t manufacturer;
    uint32_t kind;
    uint32_t version;
    uint32_t reserved[4]; // Pad to EEPROM Size
    uint32_t crc;
} ModuleKind;

typedef struct ModuleIdentity {
    fk_uuid_t id;
    uint32_t reserved[3]; // Pad to EEPROM Size
    uint32_t crc;
} ModuleIdentity;

uint32_t fk_module_kind_sign(ModuleKind const *kind);

uint32_t fk_module_kind_valid(ModuleKind const *kind);

uint32_t fk_module_identity_sign(ModuleIdentity const *identity);

uint32_t fk_module_identity_valid(ModuleIdentity const *identity);

#define FK_MODULES_MANUFACTURER                    (0x01)
#define FK_MODULES_KIND_WEATHER                    (0x01)
#define FK_MODULES_KIND_WATER                      (0x02)
#define FK_MODULES_KIND_RANDOM                     (0xa0)
#define FK_MODULES_KIND_DIAGNOSTICS                (0xa1)

#define FK_MODULES_FLAG_NONE                       (0)
#define FK_MODULES_FLAG_INTERNAL                   (1)

/**
 *
 */
#define EEPROM_I2C_ADDRESS                         (0x50)

/**
 *
 */
#define EEPROM_TIMEOUT_WRITE                       (10)

/**
 * Size in bytes of an individual page of EEPROM. This is typically the
 * maximum granularity of writes.
 */
#define EEPROM_PAGE_SIZE                           (0x20)

#define EEPROM_ADDRESS_HEADER                      (0x00)
#define EEPROM_ADDRESS_HEADER_SIZE                 (EEPROM_PAGE_SIZE * 2)
#define EEPROM_ADDRESS_CONFIG                      (EEPROM_ADDRESS_HEADER + EEPROM_ADDRESS_HEADER_SIZE)
#define EEPROM_ADDRESS_CONFIG_SIZE                 (EEPROM_PAGE_SIZE)
#define EEPROM_ADDRESS_READINGS                    (EEPROM_ADDRESS_HEADER + EEPROM_ADDRESS_HEADER_SIZE) + (EEPROM_ADDRESS_CONFIG_SIZE)

/**
 * Last address in our EEPROM, effectively the size of the EEPROM. All our
 * modules use the same chip with 64k.
 */
#define EEPROM_ADDRESS_END                         (64 * 1024)

#ifdef __cplusplus
}
#endif
