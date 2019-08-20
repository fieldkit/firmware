#pragma once

#include "networking/api_handler.h"
#include "networking/module_handler.h"
#include "networking/download_handler.h"
#include "networking/receive_handler.h"

#include "storage/storage.h"

namespace fk {

class DefaultRoutes {
private:
    ApiHandler api_handler;
    HttpRoute api{ "/fk/v1", &api_handler };

    ReceiveHandler receive_handler;
    HttpRoute receive{ "/fk/v1/upload", &receive_handler };

    DownloadHandler download_handler_data{ Storage::Data };
    DownloadHandler download_handler_meta{ Storage::Meta };
    HttpRoute downloads[4]{
        { "/fk/v1/download/0",    &download_handler_data },
        { "/fk/v1/download/data", &download_handler_data },
        { "/fk/v1/download/1",    &download_handler_meta },
        { "/fk/v1/download/meta", &download_handler_meta },
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
        router.add_route(&receive);
        router.add_route(&api);
    }

};

}
