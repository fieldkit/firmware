#pragma once

#include <modules/bridge/modules.h>

#include "networking/http_connection.h"

namespace fk {

class WaterApi {
private:

public:
    WaterApi();

public:
    bool handle(ModuleContext mc, HttpServerConnection *connection, Pool &pool);

};

} // namespace fk
