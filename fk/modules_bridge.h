#pragma once

#include "common.h"
#include "pool.h"

namespace fk {

class ModuleContext {
private:
    int32_t module_{ -1 };

public:
    ModuleContext();
    ModuleContext(ModuleContext &from, int32_t module);
    virtual ~ModuleContext();

public:
    ModuleContext module(int32_t module);

};

class ModuleReadings {
private:
    typedef struct ModuleReading {
        float value{ 0.0f };
    } ModuleReading;

    size_t nreadings_{ 0 };
    ModuleReading *readings_{ nullptr };

public:
    ModuleReadings();
    ModuleReadings(size_t n);
    virtual ~ModuleReadings();

public:
    size_t size() const;
    void set(int32_t i, float value);
    float get(int32_t i) const;

};

class Module {
private:
public:
    virtual ~Module() { }

public:
    virtual bool initialize(ModuleContext mc, Pool &pool) = 0;
    virtual ModuleReadings *take_readings(ModuleContext mc, Pool &pool) = 0;

};

/**
 *
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
    fk_module_create_fn_t ctor;
} ModuleMetadata;

}
