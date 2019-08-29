#include "modules.h"
#include "crc.h"

/**
 * Seeds the CRC of module information to avoid false positives and 0 checksums.
 */
#define MODULE_CRC_SEED  (0x838efd4)

uint32_t fk_module_kind_sign(ModuleKind const *kind) {
    return crc32_checksum(MODULE_CRC_SEED, (uint8_t *)kind, sizeof(ModuleKind) - sizeof(uint32_t));
}

uint32_t fk_module_kind_valid(ModuleKind const *kind) {
    return kind->crc == fk_module_kind_sign(kind);
}

uint32_t fk_module_identity_sign(ModuleIdentity const *identity) {
    return crc32_checksum(MODULE_CRC_SEED, (uint8_t *)identity, sizeof(ModuleIdentity) - sizeof(uint32_t));
}

uint32_t fk_module_identity_valid(ModuleIdentity const *identity) {
    return identity->crc == fk_module_identity_sign(identity);
}
