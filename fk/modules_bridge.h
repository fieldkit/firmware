#pragma once

#include "common.h"
#include "pool.h"
#include "hal/board.h"

namespace fk {

class ModuleContext {
private:
    TwoWireWrapper *module_bus_{ nullptr };
    int32_t module_{ -1 };

public:
    ModuleContext();
    ModuleContext(TwoWireWrapper &module_bus);
    ModuleContext(ModuleContext &from, int32_t module);
    virtual ~ModuleContext();

public:
    ModuleContext module(int32_t module);

public:
    TwoWireWrapper &module_bus() {
        return *module_bus_;
    }

};

class ModuleReadings {
public:
    virtual size_t size() const = 0;
    virtual void set(int32_t i, float value) = 0;
    virtual float get(int32_t i) const = 0;
};

template<size_t N>
class NModuleReadings : public ModuleReadings {
private:
    typedef struct ModuleReading {
        float value{ 0.0f };
    } ModuleReading;

    size_t nreadings_{ N };
    ModuleReading readings_[N];

public:
    NModuleReadings() : nreadings_(N) {
    }

    NModuleReadings(size_t size) : nreadings_(size) {
    }

public:
    size_t size() const override {
        return nreadings_;
    }

    void set(int32_t i, float value) override {
        readings_[i].value = value;
    }

    float get(int32_t i) const override {
        return readings_[i].value;
    }

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
