#pragma once

#include "api_handler.h"

namespace fk {

class DefaultRoutes {
private:
    ApiHandler api_v1_handler;
    HttpRoute api_v1{ "/fk/v1", &api_v1_handler };

public:
    void add_routes(HttpRouter &router) {
        router.add_route(&api_v1);
    }

};

}
