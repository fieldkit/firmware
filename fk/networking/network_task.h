#pragma once

#include "hal/hal.h"
#include "networking/server.h"
#include "state_manager.h"

namespace fk {

class NetworkTask {
private:
    Network *network_;
    HttpServer *http_server_;
    NetworkSettings active_settings_;
    uint32_t last_checked_configuration_{ 0 };
    uint32_t configuration_modified_{ 0 };

public:
    NetworkTask(Network *network, HttpServer &http_server);
    virtual ~NetworkTask();

public:
    bool begin(NetworkSettings settings, uint32_t to, Pool &pool);
    NetworkSettings get_selected_settings(Pool &pool);
    bool did_configuration_change();

};

}
