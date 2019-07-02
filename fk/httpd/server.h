#pragma once

#include "httpd/httpd.h"
#include "httpd/cpool.h"
#include "hal/wifi.h"

namespace fk {

class HttpServer {
private:
    ConnectionPool pool_;
    Wifi *wifi_;
    const char *ssid_;
    const char *password_;

public:
    HttpServer(Wifi *wifi);
    HttpServer(Wifi *wifi, const char *ssid, const char *password);

public:
    bool begin();
    void tick();
    void stop();
    bool enabled() const {
        return wifi_->enabled();
    }

private:
    WifiSettings get_settings();

};

}
