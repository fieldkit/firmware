#pragma once

#include "common.h"
#include "pool.h"
#include "hal/board.h"
#include "hal/modmux.h"

namespace fk {

class GlobalState;
class ModuleContext;

class ScanningContext {
private:
    ModMux *mm_;
    GlobalState const *gs_;
    TwoWireWrapper *module_bus_{ nullptr };

public:
    ScanningContext(ModMux *mm, GlobalState const *gs, TwoWireWrapper &module_bus);
    virtual ~ScanningContext();
    friend class ModuleContext;

public:
    ModuleContext module(int32_t position);

public:
    GlobalState const *gs();

};

class ModuleContext {
private:
    ModMux *mm_;
    GlobalState const *gs_;
    TwoWireWrapper *module_bus_{ nullptr };
    int32_t position_{ -1 };

public:
    ModuleContext(ScanningContext &from, int32_t position);
    virtual ~ModuleContext();

public:
    bool open();
    GlobalState const *gs();
    TwoWireWrapper &module_bus();
    bool power_cycle();
    uint32_t now() const;

};

class ModuleReadings {
public:
    virtual size_t size() const = 0;
    virtual void set(int32_t i, float value) = 0;
    virtual float get(int32_t i) const = 0;
    virtual ModuleReadings *clone(Pool &pool) const = 0;
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
        if ((size_t)i < N) {
            readings_[i].value = value;
        }
    }

    float get(int32_t i) const override {
        return readings_[i].value;
    }

    ModuleReadings *clone(Pool &pool) const override {
        auto clone = new (pool) NModuleReadings<N>();
        clone->nreadings_ = nreadings_;
        memcpy(clone->readings_, readings_, sizeof(readings_));
        return clone;
    }
};

/**
 * Metadata for a particular sensor.
 */
typedef struct SensorMetadata {
    const char *name;
    const char *unitOfMeasure;
    uint32_t flags;
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
    virtual ModuleSensors const *get_sensors(Pool &pool) = 0;

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
    uint32_t flags;
    fk_module_create_fn_t ctor;
} ModuleMetadata;

#ifdef __cplusplus
extern "C" {
#endif

uint32_t fk_modules_builtin_register(ModuleMetadata const *modmeta);

#ifdef __cplusplus
}
#endif

}
