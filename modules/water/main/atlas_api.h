#pragma once

#include "atlas_api_reply.h"
#include "networking/http_connection.h"

namespace fk {

class AtlasApi {
private:
    Pool *pool_;

public:
    AtlasApi();

public:
    bool handle(HttpServerConnection *connection, Pool &pool);

};

}
