#pragma once

#if defined(ARDUINO)

#include <algorithm>

#include "hal/hal.h"
#include "hal/wifi.h"

#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <ArduinoMDNS.h>

#undef min
#undef max

namespace fk {

class MetalWifiConnection : public WifiConnection {
private:
    WiFiClient wcl_;

public:
    MetalWifiConnection();
    MetalWifiConnection(WiFiClient wcl);
    ~MetalWifiConnection() override;

public:
    WifiConnectionStatus status() override;

    bool available() override;

    int32_t read(uint8_t *buffer, size_t size) override;

    int32_t write(uint8_t *buffer, size_t size) override;

    int32_t writef(const char *str, ...) override;

    int32_t write(const char *str) override;

    bool stop() override;

    int32_t socket() override;

};

class MetalWifi : public Wifi {
private:
    bool enabled_{ false };
    char service_name_[64];
    WifiSettings settings_;
    WiFiServer server_{ 80 };
    WiFiUDP udp_;
    MDNS mdns_{ udp_ };

public:
    bool begin(WifiSettings settings) override;

    WifiStatus status() override;

    bool serve() override;

    uint32_t ip_address() override;

    WifiConnection *accept() override;

    bool stop() override;

    bool enabled() override;

};

}

#endif
