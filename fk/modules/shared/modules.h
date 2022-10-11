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
    uint32_t crc;
} ModuleHeader;

uint32_t fk_module_header_sign(ModuleHeader const *header);

uint32_t fk_module_header_valid(ModuleHeader const *header);

uint8_t fk_module_is_available_i2c_address(uint8_t address);

#define FKB_FLAGS_NONE                             (0x0)
#define FKB_FLAGS_DYNAMIC                          (0x1)

#define FK_MODULES_MANUFACTURER                    (0x01)
#define FK_MODULES_KIND_WEATHER                    (0x01)
#define FK_MODULES_KIND_UNUSED                     (0x02)
#define FK_MODULES_KIND_DISTANCE                   (0x03)
#define FK_MODULES_KIND_ATLAS_PH                   (0x04)
#define FK_MODULES_KIND_ATLAS_EC                   (0x05)
#define FK_MODULES_KIND_ATLAS_DO                   (0x06)
#define FK_MODULES_KIND_ATLAS_TEMP                 (0x07)
#define FK_MODULES_KIND_ATLAS_ORP                  (0x08)
#define FK_MODULES_KIND_WATER_PH                   (0x09)
#define FK_MODULES_KIND_WATER_EC                   (0x10)
#define FK_MODULES_KIND_WATER_DO                   (0x11)
#define FK_MODULES_KIND_WATER_TEMP                 (0x12)
#define FK_MODULES_KIND_WATER_ORP                  (0x13)
#define FK_MODULES_KIND_WATER_OMNI                 (0x14)

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

/**
 * Last address in our EEPROM, effectively the size of the EEPROM. All our
 * modules use the same chip with 8k.
 */
#define EEPROM_ADDRESS_END                         (8 * 1024)

#define EEPROM_ADDRESS_HEADER                      (0x00)
#define EEPROM_ADDRESS_HEADER_SIZE                 (EEPROM_PAGE_SIZE * 2)
#define EEPROM_ADDRESS_CONFIG                      (EEPROM_ADDRESS_HEADER + EEPROM_ADDRESS_HEADER_SIZE)
#define EEPROM_ADDRESS_CONFIG_SIZE                 (EEPROM_PAGE_SIZE)
#define EEPROM_ADDRESS_READINGS                    (EEPROM_ADDRESS_HEADER + EEPROM_ADDRESS_HEADER_SIZE) + (EEPROM_ADDRESS_CONFIG_SIZE)
#define EEPROM_ADDRESS_READINGS_END                (EEPROM_ADDRESS_END)
#define EEPROM_AVAILABLE_DATA                      (EEPROM_ADDRESS_END - EEPROM_ADDRESS_CONFIG_SIZE - EEPROM_ADDRESS_HEADER_SIZE)

/**
 * Seeds the CRC of module information to avoid false positives and 0 checksums.
 */
#define FK_MODULES_CRC_SEED                        (0x838efd4)

/**
 * Maximum number of ms we allow for EEPROM writes on the module side. We wait
 * this long after acquiring the eeprom protect line to ensure any writes that
 * were in flight complete before we use the bus.
 */
#define FK_MODULES_EEPROM_WRITE_TIME               (300)

#define FK_MODULES_EEPROM_WARNING_WINDOW           (20)
#define FK_MODULES_EEPROM_WARNING_TICKS            (2)
#define FK_MODULES_EEPROM_WARNING_SLEEP_PER_TICK   (500)

#ifdef __cplusplus
}
#endif
