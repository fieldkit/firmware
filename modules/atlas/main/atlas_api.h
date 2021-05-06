#pragma once

#include "atlas_api_reply.h"
#include "networking/http_connection.h"
#include "oem_atlas.h"

namespace fk {

class AtlasApi {
private:
    AtlasSensorType type_{ AtlasSensorType::Unknown };
    OemAtlas *atlas_{ nullptr };

public:
    AtlasApi(AtlasSensorType type, OemAtlas &atlas);

public:
    bool handle(ModuleContext mc, HttpServerConnection *connection, Pool &pool);

private:
    bool send_reply(HttpStatus status_code, HttpServerConnection *connection, Pool &pool, AtlasApiReply &reply);
    bool status(ModuleContext mc, AtlasApiReply &reply, Pool &pool);
    bool clear(ModuleContext mc, AtlasApiReply &reply, Pool &pool);
    bool calibrate(ModuleContext mc, AtlasApiReply &reply, fk_atlas_AtlasCalibrationCommand command, Pool &pool);

};

} // namespace fk
