#pragma once

#include "networking/api_handler.h"
#include "networking/module_handler.h"
#include "networking/download_handler.h"
#include "networking/receive_firmware_handler.h"
#include "networking/logs_handler.h"

#include "storage/storage.h"

namespace fk {

class DefaultRoutes {
private:
    ApiHandler api_handler;
    HttpRoute api{ "/fk/v1", &api_handler };

    ReceiveFirmwareHandler receive_firmware_handler;
    HttpRoute receive_firmware{ "/fk/v1/upload/firmware", &receive_firmware_handler };

    DownloadLogsHandler download_logs_handler;
    DownloadHandler download_handler_data{ Storage::Data };
    DownloadHandler download_handler_meta{ Storage::Meta };
    HttpRoute downloads[3]{
        { "/fk/v1/download/logs", &download_logs_handler },
        { "/fk/v1/download/data", &download_handler_data },
        { "/fk/v1/download/meta", &download_handler_meta },
    };

    ModuleHandler module_handlers[4]{
        { ModulePosition::from(0) },
        { ModulePosition::from(1) },
        { ModulePosition::from(2) },
        { ModulePosition::from(3) },
    };
    HttpRoute modules[4]{
        { "/fk/v1/module/0", &module_handlers[0] },
        { "/fk/v1/module/1", &module_handlers[1] },
        { "/fk/v1/module/2", &module_handlers[2] },
        { "/fk/v1/module/3", &module_handlers[3] },
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
        router.add_route(&downloads[2]);
        router.add_route(&receive_firmware);
        router.add_route(&api);
    }

};

}
