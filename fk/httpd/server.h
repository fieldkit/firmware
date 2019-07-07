#pragma once

#include "hal/hal.h"
#include "httpd/req.h"
#include "httpd/cpool.h"
#include "httpd/server.h"
#include "httpd/routing.h"

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

public:
    bool begin();
    void tick();
    void stop();
    bool enabled() const {
        return network_->enabled();
    }

private:
    NetworkSettings get_settings();

};

}
