#pragma once

#include "config.h"
#include "hal/hal.h"
#include "networking/req.h"
#include "networking/cpool.h"
#include "networking/server.h"
#include "networking/routing.h"

namespace fk {

class NetworkRunningCallback {
public:
    virtual bool running() = 0;

};

class HttpServer {
private:
    ConnectionPool pool_;
    HttpRouter router_;
    Network *network_;
    configuration_t const *fkc_;

public:
    HttpServer(Network *network, configuration_t const *fkc);
    ~HttpServer();

public:
    bool begin(uint32_t listening_to, NetworkRunningCallback *callback);

    void tick();

    void stop();

public:
    bool enabled() const {
        return network_->enabled();
    }

    uint32_t activity() const {
        return pool_.activity();
    }

    bool active_connections() const {
        return pool_.active_connections();
    }

private:
    bool try_configurations(const char *name);

    NetworkSettings get_settings(configuration_t::wifi_network_t const &network, const char *name);

};

}
