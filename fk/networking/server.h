#pragma once

#include "config.h"
#include "hal/hal.h"
#include "networking/req.h"
#include "networking/cpool.h"
#include "networking/server.h"
#include "networking/routing.h"

namespace fk {

class HttpServer {
private:
    ConnectionPool pool_;
    HttpRouter router_;
    Network *network_;
    NetworkSettings settings_;
    configuration_t const *fkc_;

public:
    HttpServer(Network *network, configuration_t const *fkc);
    ~HttpServer();

public:
    bool begin(uint32_t to, Pool &pool);

    bool begin(NetworkSettings settings, uint32_t to, Pool &pool);

    bool serve();

    void tick();

    void stop();

public:
    bool enabled() const;

    uint32_t activity() const;

    bool active_connections() const;

    bool ready_to_serve() const;

    const char *ssid() const;

private:
    bool try_configurations(const char *name, uint32_t to, Pool &pool);

    NetworkSettings get_settings(configuration_t::wifi_network_t const &network, const char *name);

};

}
