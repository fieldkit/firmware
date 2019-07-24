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
    Network *network_;
    HttpRouter router_;
    const char *ssid_;
    const char *password_;

public:
    HttpServer(Network *network);
    HttpServer(Network *network, const char *ssid, const char *password);
    ~HttpServer();

public:
    bool begin();

    void tick();

    void stop();

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
    NetworkSettings get_settings();

};

}
