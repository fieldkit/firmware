#pragma once

#include "common.h"

namespace fk {

class ModuleContext {
private:

public:
    ModuleContext();
    virtual ~ModuleContext();

public:

};

class Module {
private:
public:
    virtual ~Module() { }

public:
    virtual bool initialize(ModuleContext &mc) = 0;
    virtual bool take_readings(ModuleContext &mc) = 0;

};

/**
 *
 */
typedef Module*(*fk_module_create_fn_t)();

/**
 * Metadata for a particular module. This is stored in a registry.
 */
typedef struct ModuleMetadata {
    uint32_t manufacturer;
    uint32_t kind;
    uint32_t version;
    const char *name;
    fk_module_create_fn_t ctor;
} ModuleMetadata;

}
