#pragma once

#if defined(ARDUINO)

#include "exchange.h"
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
    bool debugging_{ false };
    uint8_t *buffer_{ nullptr };
    size_t size_{ 0 };
    size_t position_{ 0 };

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

class MetalNetworkListener : public NetworkListener {
private:
    uint16_t port_;
    WiFiServer server_{ port_ };

public:
    MetalNetworkListener(uint16_t port);

public:
    bool begin();

    PoolPointer<NetworkConnection> *accept() override;

    bool stop() override;

};

class MetalNetwork : public Network {
private:
    Pool *pool_{ nullptr };
    NetworkSettings settings_;
    bool enabled_{ false };
    bool serving_{ false };
    char mdns_name_[64];
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

    PoolPointer<NetworkListener> *listen(uint16_t port) override;

    void service() override;

    PoolPointer<NetworkConnection> *open_connection(const char *scheme, const char *hostname, uint16_t port) override;

    bool stop() override;

    bool enabled() override;

    bool synchronize_time() override;

    bool get_mac_address(uint8_t *address) override;

    const char *get_ssid() override;

    bool get_created_ap() override;

};

FK_ENABLE_TYPE_NAME(MetalNetworkConnection);
FK_ENABLE_TYPE_NAME(MetalNetworkListener);

}

#endif
