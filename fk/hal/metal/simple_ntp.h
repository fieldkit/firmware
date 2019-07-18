#pragma once

#if defined(ARDUINO)

#include <WiFi101.h>
#include <WiFiUdp.h>

#include "common.h"

namespace fk {

class Wifi;

class SimpleNTP {
    static constexpr size_t NumberOfAddresses = 2;

private:
    WiFiUDP udp_;
    IPAddress addresses_[NumberOfAddresses] {
        IPAddress{129, 6, 15, 28},
        IPAddress{164, 67, 62, 194}
    };
    uint8_t index_{ 0 };
    bool initialized_{ false };
    uint32_t queried_{ 0 };
    uint32_t synced_{ 0 };

public:
    SimpleNTP();
    virtual ~SimpleNTP();

public:
    void start();
    bool service();
    void stop();

private:
    bool send();

};

}

#endif // defined(ARDUINO)
