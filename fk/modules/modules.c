#include "modules.h"
#include "crc.h"

uint32_t fk_module_header_sign(ModuleHeader *header) {
    return crc32_checksum((uint8_t *)header, sizeof(ModuleHeader) - sizeof(uint32_t));
}

uint32_t fk_module_header_valid(ModuleHeader *header) {
    return header->crc == fk_module_header_sign(header);
}

static ModuleNode nodes[FK_MODULES_BUILTIN_MAXIMUM];

uint32_t fk_modules_builtin_register(ModuleMetadata const *modmeta) {
    ModuleNode *prev = NULL;

    for (int32_t i = 0; i < FK_MODULES_BUILTIN_MAXIMUM; ++i) {
        if (nodes[i].meta == NULL) {
            nodes[i].meta = modmeta;
            nodes[i].link = NULL;
            if (prev != NULL) {
                prev->link = &nodes[i];
            }
            break;
        }
        prev = &nodes[i];
    }
    return 0;
}

uint32_t fk_modules_builtin_get(ModuleNode **iter) {
    if (nodes[0].meta == NULL) {
        *iter = NULL;
        return 0;
    }
    *iter = &nodes[0];
    return 0;
}
