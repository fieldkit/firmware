#pragma once

#include "atlas_api_reply.h"
#include "networking/http_connection.h"
#include "oem_atlas.h"

namespace fk {

class AtlasApi {
private:
    AtlasSensorType type_{ AtlasSensorType::Unknown };
    Pool *pool_;

public:
    AtlasApi(AtlasSensorType type);

public:
    bool handle(HttpServerConnection *connection, Pool &pool);

};

}
