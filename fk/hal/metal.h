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

constexpr uint8_t WINC1500_CS = 54u;  /* This is moving */
constexpr uint8_t WINC1500_ENABLE = 95u;
constexpr uint8_t WINC1500_IRQ = 97u;
constexpr uint8_t WINC1500_RESET = 96u;
constexpr uint8_t WINC1500_POWER = 56u;

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

    int32_t write(const char *str) override;

    bool stop() override;

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
