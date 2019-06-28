#pragma once

// NOTE: I'd like to push this into an implementation. 
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <ArduinoMDNS.h>

#include "httpd/httpd.h"

namespace fk {

constexpr uint8_t WINC1500_CS = 54u;  /* This is moving */
constexpr uint8_t WINC1500_ENABLE = 95u;
constexpr uint8_t WINC1500_IRQ = 97u;
constexpr uint8_t WINC1500_RESET = 96u;
constexpr uint8_t WINC1500_POWER = 56u;

class HttpServer {
private:
    const char *ssid_;
    const char *password_;
    WiFiServer server_{ 80 };
    HttpRouter router_;
    WiFiUDP udp_;
    MDNS mdns_{ udp_ };

public:
    HttpServer();
    HttpServer(const char *ssid, const char *password);

public:
    bool begin();
    void tick();

};

}
