#pragma once

/**
 * RADIO_MOSI  PB26
 * RADIO_MISO  PB29
 * RADIO_SCK   PB27
 *
 * WIFI_CS     PB05
 * WIFI_ENABLE PC25
 * WIFI_RESET  PC26
 * WIFI_IRQ    PC27
 *
 * WIFI_POWER  PB07
 *
 * GPS_TX      PB24
 * GPS_RX      PB25
 * GPS_POWER   PB06
 *
 * Wire1
 * RAD_SDA     PC22
 * RAD_SCL     PC23
 */

#if defined(ARDUINO)

#include "hal/hal.h"

#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <ArduinoMDNS.h>

#undef min
#undef max

namespace fk {

class MetalNetworkConnection : public NetworkConnection {
private:
    WiFiClient wcl_;

public:
    MetalNetworkConnection();
    MetalNetworkConnection(WiFiClient wcl);
    ~MetalNetworkConnection() override;

public:
    NetworkConnectionStatus status() override;

    bool available() override;

    int32_t read(uint8_t *buffer, size_t size) override;

    int32_t write(const uint8_t *buffer, size_t size) override;

    int32_t writef(const char *str, ...) override;

    int32_t write(const char *str) override;

    bool stop() override;

    int32_t socket() override;

};

class MetalNetwork : public Network {
private:
    bool enabled_{ false };
    char service_name_[64];
    NetworkSettings settings_;
    WiFiServer server_{ 80 };
    WiFiUDP udp_;
    MDNS mdns_{ udp_ };

public:
    bool begin(NetworkSettings settings) override;

    NetworkStatus status() override;

    bool serve() override;

    uint32_t ip_address() override;

    NetworkConnection *accept() override;

    bool stop() override;

    bool enabled() override;

};

}

#endif
