#pragma once

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Maximum number of modules that can be builtin to the firmware.
 */
#define FK_MODULES_BUILTIN_MAXIMUM (10)

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

/**
 * Metadata for a particular module. This is stored in a registry.
 */
typedef struct ModuleMetadata {
    uint32_t manufacturer;
    uint32_t kind;
    uint32_t version;
    const char *name;
} ModuleMetadata;

typedef struct ModuleNode {
    ModuleMetadata const *meta;
    struct ModuleNode *link;
} ModuleNode;

uint32_t fk_modules_builtin_register(ModuleMetadata const *modmeta);

uint32_t fk_modules_builtin_get(ModuleNode **iter);

#ifdef __cplusplus
}
#endif
