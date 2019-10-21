#pragma once

#include "config.h"
#include "hal/hal.h"
#include "networking/req.h"
#include "networking/cpool.h"
#include "networking/server.h"
#include "networking/routing.h"
#include "state.h"

namespace fk {

class HttpServer {
private:
    ConnectionPool pool_;
    HttpRouter router_;
    Network *network_;
    NetworkSettings settings_;

public:
    HttpServer(Network *network);
    virtual ~HttpServer();

public:
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

    uint32_t bytes_rx() const {
        return pool_.bytes_rx();
    };

    uint32_t bytes_tx() const {
        return pool_.bytes_tx();
    };

};

}
