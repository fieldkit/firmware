#pragma once

#if !defined(ARDUINO)

#include "hal/hal.h"

namespace fk {

class LinuxNetworkConnection : public NetworkConnection {
private:
    int32_t s_;
    uint32_t remote_address_;

public:
    LinuxNetworkConnection();
    LinuxNetworkConnection(int32_t s, uint32_t remote_address);
    ~LinuxNetworkConnection() override;

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

class LinuxNetworkListener : public NetworkListener {
private:
    uint16_t port_;
    int32_t listening_;

public:
    LinuxNetworkListener(uint16_t port, int32_t listening);

public:
    PoolPointer<NetworkConnection> *accept() override;

    bool stop() override;

};

class LinuxNetwork : public Network {
private:
    bool enabled_{ false };
    NetworkSettings settings_;
    int32_t listening_;

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

};

FK_ENABLE_TYPE_NAME(LinuxNetworkConnection);
FK_ENABLE_TYPE_NAME(LinuxNetworkListener);

}

#endif
