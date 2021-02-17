#pragma once

#include "atlas_api_reply.h"
#include "networking/http_connection.h"
#include "oem_atlas.h"

namespace fk {

class AtlasApi {
private:
    AtlasSensorType type_{ AtlasSensorType::Unknown };
    OemAtlas *atlas_;
    Pool *pool_;

public:
    AtlasApi(AtlasSensorType type, OemAtlas &atlas);

public:
    bool handle(ModuleContext mc, HttpServerConnection *connection, Pool &pool);

private:
    bool send_reply(HttpServerConnection *connection, Pool &pool, AtlasApiReply &reply);
    bool status(ModuleContext mc, AtlasApiReply &reply);
    bool clear(ModuleContext mc, AtlasApiReply &reply);
    bool calibrate(ModuleContext mc, AtlasApiReply &reply, fk_atlas_AtlasCalibrationCommand command);

};

} // namespace fk
