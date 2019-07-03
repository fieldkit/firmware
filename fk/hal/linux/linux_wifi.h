#pragma once

#if !defined(ARDUINO)

#include "hal/hal.h"

namespace fk {

class LinuxWifiConnection : public WifiConnection {
private:
    int32_t s_;

public:
    LinuxWifiConnection();
    LinuxWifiConnection(int32_t s);
    ~LinuxWifiConnection() override;

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

class LinuxWifi : public Wifi {
private:
    bool enabled_{ false };
    char service_name_[64];
    WifiSettings settings_;
    int32_t listening_;

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
