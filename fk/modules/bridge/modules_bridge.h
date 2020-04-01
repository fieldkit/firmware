#pragma once

#include "common.h"
#include "pool.h"
#include "hal/board.h"
#include "hal/modmux.h"

#include "networking/http_connection.h"

#include "modules/bridge/data.h"
#include "modules/bridge/contexts.h"

namespace fk {

class GlobalState;
class ModuleContext;
class HttpServerConnection;

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
 * Generic return value for Module bridge calls.
 */
struct ModuleReturn {
    ModuleStatus status;

    ModuleReturn(ModuleStatus status) : status(status) {
    }

    /**
     * Returns true if the Module is A-ok
     */
    operator bool() const {
        return status == ModuleStatus::Ok;
    }
};

/**
 * Primary module interface.
 */
class Module {
private:
public:
    virtual ~Module() { }

public:
    virtual ModuleReturn initialize(ModuleContext mc, Pool &pool) = 0;
    virtual ModuleReadings *take_readings(ModuleContext mc, Pool &pool) = 0;
    virtual ModuleSensors const *get_sensors(Pool &pool) = 0;
    virtual ModuleConfiguration get_configuration(Pool &pool) = 0;
    virtual ModuleReturn service(ModuleContext mc, Pool &pool) = 0;
    virtual ModuleReturn api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) = 0;

};

#ifdef __cplusplus
extern "C" {
#endif

uint32_t fk_modules_builtin_register(ModuleMetadata const *modmeta);

#ifdef __cplusplus
}
#endif

}
