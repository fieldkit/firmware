#pragma once

#if defined(ARDUINO)

#include "hal/hal.h"
#include "hal/metal/simple_ntp.h"

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
    virtual ~MetalNetworkConnection() override;

public:
    NetworkConnectionStatus status() override;

    bool available() override;

    int32_t read(uint8_t *buffer, size_t size) override;

    int32_t write(const uint8_t *buffer, size_t size) override;

    int32_t writef(const char *str, ...) override;

    int32_t vwritef(const char *str, va_list args) override;

    int32_t write(const char *str) override;

    int32_t socket() override;

    uint32_t remote_address() override;

    bool stop() override;

};

class MetalNetwork : public Network {
private:
    NetworkSettings settings_;
    bool enabled_{ false };
    char service_name_[64];
    uint32_t registered_{ 0 };
    WiFiServer server_{ 80 };
    WiFiUDP udp_;
    MDNS mdns_{ udp_ };
    SimpleNTP ntp_;

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
