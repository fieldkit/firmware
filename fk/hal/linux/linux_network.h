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

class LinuxNetwork : public Network {
private:
    bool enabled_{ false };
    char service_name_[64];
    NetworkSettings settings_;
    int32_t listening_;

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
