#pragma once

#include "httpd.h"

namespace fk {

class HttpServer {
private:
    ConnectionPool pool_;
    Network *network_;
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
