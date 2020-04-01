#pragma once

namespace fk {

class Module;

/**
 * Callback function that creates an instance of a Module.
 */
typedef Module*(*fk_module_create_fn_t)(Pool &pool);

/**
 * Metadata for a particular module. This is stored in a registry.
 */
typedef struct ModuleMetadata {
    uint32_t manufacturer;
    uint32_t kind;
    uint32_t version;
    const char *name;
    uint32_t flags;
    fk_module_create_fn_t ctor;
} ModuleMetadata;

}
