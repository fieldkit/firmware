#pragma once

#include <modules/bridge/modules.h>

#include "water_api_reply.h"
#include "networking/http_connection.h"

namespace fk {

class OmniWaterApi {
public:
    OmniWaterApi();

public:
    bool handle(ModuleContext mc, HttpServerConnection *connection, Pool &pool);

private:
    bool send_reply(HttpStatus status_code, HttpServerConnection *connection, WaterApiReply &reply, Pool &pool);
    bool status(ModuleContext mc, WaterApiReply &reply, Pool &pool);
    bool clear(ModuleContext mc, WaterApiReply &reply, Pool &pool);
    bool calibrate(ModuleContext mc, WaterApiReply &reply, fk_app_ModuleHttpQuery *query, Pool &pool);
};

} // namespace fk
