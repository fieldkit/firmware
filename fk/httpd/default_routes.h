#pragma once

#include "api_handler.h"
#include "module_handler.h"
#include "download_handler.h"

namespace fk {

class DefaultRoutes {
private:
    ApiHandler api_handler;
    HttpRoute api{ "/fk/v1", &api_handler };

    DownloadHandler download_handler;
    HttpRoute downloads[2]{
        { "/fk/v1/download/0", &download_handler },
        { "/fk/v1/download/1", &download_handler },
    };

    ModuleHandler module_handler;
    HttpRoute modules[4]{
        { "/fk/v1/module/0", &module_handler },
        { "/fk/v1/module/1", &module_handler },
        { "/fk/v1/module/2", &module_handler },
        { "/fk/v1/module/4", &module_handler },
    };

public:
    void add_routes(HttpRouter &router) {
        // NOTE: Order is important here.
        router.add_route(&modules[0]);
        router.add_route(&modules[1]);
        router.add_route(&modules[2]);
        router.add_route(&modules[3]);
        router.add_route(&downloads[0]);
        router.add_route(&downloads[1]);
        router.add_route(&api);
    }

};

}
