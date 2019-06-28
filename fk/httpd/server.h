#pragma once

#include "httpd/httpd.h"
#include "hal/wifi.h"

namespace fk {

class HttpServer {
private:
    Wifi *wifi_;
    const char *ssid_;
    const char *password_;

public:
    HttpServer(Wifi *wifi);
    HttpServer(Wifi *wifi, const char *ssid, const char *password);

public:
    bool begin();
    void tick();

private:
    WifiSettings get_settings();

};

}
