#pragma once

#include "networking/api_handler.h"
#include "networking/module_handler.h"
#include "networking/download_handler.h"
#include "networking/receive_firmware_handler.h"
#include "networking/write_memory_handler.h"
#include "networking/logs_handler.h"
#include "networking/file_handler.h"

#include "storage/storage.h"

namespace fk {

class DefaultRoutes {
private:
    ApiHandler api_handler;
    HttpRoute api{ "/fk/v1", &api_handler };

    ReceiveFirmwareHandler receive_firmware_handler;
    HttpRoute receive_firmware{ "/fk/v1/upload/firmware", &receive_firmware_handler };

    WriteMemoryHandler write_memory_handler;
    HttpRoute write_memory{ "/fk/v1/memory/qspi", &write_memory_handler };

    DownloadLogsHandler download_handler_logs;
    DownloadFileHandler download_handler_file;
    DownloadHandler download_handler_data{ Storage::Data };
    DownloadHandler download_handler_meta{ Storage::Meta };
    HttpRoute downloads[4]{
        { "/fk/v1/download/logs", &download_handler_logs },
        { "/fk/v1/download/data", &download_handler_data },
        { "/fk/v1/download/meta", &download_handler_meta },
        { "/fk/v1/download/file", &download_handler_file },
    };

    ModuleHandler module_handlers[5]{
        { ModulePosition::from(0) },
        { ModulePosition::from(1) },
        { ModulePosition::from(2) },
        { ModulePosition::from(3) },
        { ModulePosition::from(4) },
    };

    HttpRoute modules[5]{
        { "/fk/v1/modules/0", &module_handlers[0] },
        { "/fk/v1/modules/1", &module_handlers[1] },
        { "/fk/v1/modules/2", &module_handlers[2] },
        { "/fk/v1/modules/3", &module_handlers[3] },
        { "/fk/v1/modules/4", &module_handlers[4] },
    };

public:
    void add_routes(HttpRouter &router) {
        // NOTE: Order is important here.
        router.add_route(&modules[0]);
        router.add_route(&modules[1]);
        router.add_route(&modules[2]);
        router.add_route(&modules[3]);
        router.add_route(&modules[4]);
        router.add_route(&downloads[0]);
        router.add_route(&downloads[1]);
        router.add_route(&downloads[2]);
        router.add_route(&downloads[3]);
        router.add_route(&receive_firmware);
        router.add_route(&write_memory);
        router.add_route(&api);
    }

};

}
