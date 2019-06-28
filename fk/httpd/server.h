#pragma once

#include "httpd/httpd.h"
#include "hal/wifi.h"

namespace fk {

class HttpServer {
private:
    const char *ssid_;
    const char *password_;
    Wifi *wifi_;

public:
    HttpServer();
    HttpServer(const char *ssid, const char *password);

public:
    bool begin();
    void tick();

private:
    WifiSettings get_settings();

};

}
