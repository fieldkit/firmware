#pragma once

#include "fk.h"

namespace fk {

enum class WifiStatus {
    Error,
    Ready,
    Off,
    Listening,
    Connected
};

enum class WifiConnectionStatus {
    Connected,
    Disconnected
};

struct WifiSettings {
    bool create;
    const char *ssid;
    const char *password;
    const char *name;
    uint16_t port;
};

class WifiConnection {
public:
    virtual WifiConnectionStatus status() = 0;

    virtual bool available() = 0;

    virtual int32_t read(uint8_t *buffer, size_t size) = 0;

    virtual int32_t write(uint8_t *buffer, size_t size) = 0;

    virtual int32_t write(const char *str) = 0;

    virtual bool stop() = 0;

};

class Wifi {
public:
    virtual bool begin(WifiSettings settings) = 0;

    virtual bool serve() = 0;

    virtual WifiStatus status() = 0;

    virtual uint32_t ip_address() = 0;

    virtual WifiConnection *accept() = 0;

    virtual bool stop() = 0;

};

}
