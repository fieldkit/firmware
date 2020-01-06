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
    bool handle(HttpServerConnection *connection, Pool &pool);

private:
    bool handle(HttpServerConnection *connection, Pool &pool, AtlasApiReply &reply);
    bool status(AtlasApiReply &reply);
    bool clear(AtlasApiReply &reply);
    bool set(AtlasApiReply &reply);

};

}
