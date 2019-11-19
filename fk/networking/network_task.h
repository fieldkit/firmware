#pragma once

#include "hal/hal.h"
#include "state_manager.h"
#include "networking/network_services.h"

namespace fk {

class NetworkTask {
private:
    Network *network_;
    NetworkServices *network_services_;
    NetworkSettings active_settings_;
    uint32_t last_checked_configuration_{ 0 };
    uint32_t configuration_modified_{ 0 };

public:
    NetworkTask(Network *network, NetworkServices &network_services);
    virtual ~NetworkTask();

public:
    bool begin(NetworkSettings settings, uint32_t to, Pool &pool);
    NetworkSettings get_selected_settings(Pool &pool);
    bool did_configuration_change();

};

}
