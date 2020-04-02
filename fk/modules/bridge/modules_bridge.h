#pragma once

#include "common.h"
#include "pool.h"
#include "hal/board.h"
#include "hal/modmux.h"

#include "networking/http_connection.h"

#include "modules/bridge/data.h"
#include "modules/bridge/contexts.h"
#include "modules/bridge/module_readings.h"

namespace fk {

class ModuleContext;
class HttpServerConnection;

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
    virtual ModuleReadings *take_readings(ReadingsContext mc, Pool &pool) = 0;
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
