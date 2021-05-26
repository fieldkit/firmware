#pragma once

#include "config.h"
#include "hal/hal.h"
#include "networking/req.h"
#include "networking/cpool.h"
#include "networking/routing.h"
#include "state.h"

namespace fk {

class NetworkServices {
private:
    HttpRouter router_;
    ConnectionPool pool_{ router_ };
    Network *network_{ nullptr };
    PoolPointer<NetworkListener> *http_listener_{ nullptr };
    PoolPointer<NetworkListener> *debug_listener_{ nullptr };
    NetworkSettings active_settings_;
    uint32_t last_checked_configuration_{ 0 };
    uint32_t configuration_modified_{ 0 };

public:
    NetworkServices(Network *network);
    virtual ~NetworkServices();

public:
    bool try_begin(NetworkSettings settings, uint32_t to, Pool &pool);

    bool serve();

    void tick(Pool *pool);

    void stop();

public:
    bool enabled() const;

    uint32_t activity() const;

    bool active_connections() const;

    bool ready_to_serve() const;

    const char *ssid() const;

    uint32_t bytes_rx() const {
        return pool_.bytes_rx();
    };

    uint32_t bytes_tx() const {
        return pool_.bytes_tx();
    };

public:
    bool begin(NetworkSettings settings, uint32_t to, Pool &pool);
    NetworkSettings get_selected_settings(Pool &pool);
    bool did_configuration_change();

};

}
