#pragma once

#include "common.h"
#include "pool.h"
#include "hal/board.h"

namespace fk {

class GlobalState;

class ModuleContext {
private:
    GlobalState const *gs_;
    TwoWireWrapper *module_bus_{ nullptr };
    int32_t module_{ -1 };

public:
    ModuleContext();
    ModuleContext(GlobalState const *gs, TwoWireWrapper &module_bus);
    ModuleContext(ModuleContext &from, int32_t module);
    virtual ~ModuleContext();

public:
    ModuleContext module(int32_t module);

public:
    GlobalState const *gs() {
        return gs_;
    }

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
        FK_ASSERT(size <= N);
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

/**
 * Metadata for a particular sensor.
 */
typedef struct SensorMetadata {
    const char *name;
    const char *unitOfMeasure;
} SensorMetadata;

/**
 * Information on all the sensors attached to a module.
 */
typedef struct ModuleSensors {
    size_t nsensors;
    SensorMetadata const *sensors;
} ModuleSensors;

/**
 * Primary module interface.
 */
class Module {
private:
public:
    virtual ~Module() { }

public:
    virtual bool initialize(ModuleContext mc, Pool &pool) = 0;
    virtual ModuleReadings *take_readings(ModuleContext mc, Pool &pool) = 0;
    virtual ModuleSensors const *get_sensors(ModuleContext mc, Pool &pool) = 0;

};

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
    fk_module_create_fn_t ctor;
} ModuleMetadata;

}
