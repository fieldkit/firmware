#pragma once

#include "config.h"
#include "hal/hal.h"
#include "state.h"
#include "networking/req.h"
#include "networking/cpool.h"
#include "networking/routing.h"
#include "networking/network_duration.h"

namespace fk {

class NetworkServices {
private:
    Pool *tick_pool_{ nullptr };
    HttpRouter router_;
    ConnectionPool connection_pool_{ router_ };
    Network *network_{ nullptr };
    PoolPointer<NetworkListener> *http_listener_{ nullptr };
    PoolPointer<NetworkListener> *debug_listener_{ nullptr };
    NetworkSettings active_settings_;
    uint32_t last_checked_configuration_{ 0 };
    uint32_t configuration_modified_{ 0 };
    NetworkDuration duration_;
    uint32_t signal_checked_{ 0u };
    uint32_t started_{ 0 };
    bool serving_{ false };

public:
    NetworkServices(Network *network, Pool &pool);
    virtual ~NetworkServices();

public:
    bool try_begin(NetworkSettings settings, uint32_t to, Pool &pool);

    bool serve();

    void tick();

    void stop();

public:
    bool enabled() const;

    const char *ssid() const;

    bool active_connections() const;

    uint32_t activity() const;

    uint32_t bytes_rx() const;

    uint32_t bytes_tx() const;

    bool waiting_to_serve();

    bool can_serve() const;

    bool serving();

    bool should_stop();

public:
    bool begin(uint32_t to, Pool &pool);

private:
    NetworkSettings get_selected_settings(Pool &pool);
    bool did_configuration_change();

};

}
