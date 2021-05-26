#pragma once

#if defined(__SAMD51__)

#include "exchange.h"
#include "hal/hal.h"
#include "hal/metal/simple_ntp.h"
#include "hal/metal/udp_discovery.h"
#include "hal/metal/mdns.h"

#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <WiFiSocket.h>

#undef min
#undef max

namespace fk {

struct WifiBufferedWriter {
    uint8_t buffer[256];
    size_t buffer_size{ 256 };
    size_t position{ 0 };
    WiFiClient *wcl;
    int32_t return_value{ 0 };
    int32_t flush();
};

class MetalNetworkConnection : public NetworkConnection {
private:
    WiFiClient wcl_;
    WifiBufferedWriter *buffered_writer_{ nullptr };
    bool debugging_{ false };
    uint8_t *buffer_{ nullptr };
    size_t size_{ 0 };
    size_t position_{ 0 };

public:
    MetalNetworkConnection();
    MetalNetworkConnection(Pool *pool, WiFiClient wcl);
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
    MetalNetworkListener(Pool *pool, uint16_t port);

public:
    bool begin();

    PoolPointer<NetworkConnection> *accept() override;

    bool stop() override;

};

class StaticWiFiCallbacks : public WiFiCallbacks {
private:
    constexpr static size_t ExpectedWiFiBufferSize = 1472;
    constexpr static size_t NumberOfBuffers = 3;

    struct Buffer {
        bool taken{ false };
        void *ptr{ nullptr };
    };
    Buffer buffers_[NumberOfBuffers];

public:
    void initialize(Pool &pool);
    void *malloc(size_t size) override;
    void free(void *ptr) override;
    bool busy(uint32_t elapsed) override;

};

class MetalNetwork : public Network {
private:
    Availability availability_{ Availability::Unknown };
    Pool *pool_{ nullptr };
    NetworkSettings settings_;
    bool enabled_{ false };
    bool serving_{ false };
    WiFiServer server_{ 80 };
    MDNSDiscovery mdns_discovery_;
    UDPDiscovery udp_discovery_;
    SimpleNTP ntp_;

public:
    bool begin(NetworkSettings settings, Pool *pool) override;

    NetworkStatus status() override;

    bool serve() override;

    uint32_t ip_address() override;

    int32_t rssi() override;

    PoolPointer<NetworkListener> *listen(uint16_t port) override;

    void service(Pool *pool) override;

    PoolPointer<NetworkConnection> *open_connection(const char *scheme, const char *hostname, uint16_t port) override;

    bool stop() override;

    bool enabled() override;

    bool synchronize_time() override;

    bool get_mac_address(uint8_t *address) override;

    const char *get_ssid() override;

    bool get_created_ap() override;

    NetworkScan scan(Pool &pool) override;

    void verify() override;
};

FK_ENABLE_TYPE_NAME(MetalNetworkConnection);
FK_ENABLE_TYPE_NAME(MetalNetworkListener);

}

#endif
