#pragma once

#if defined(__SAMD51__)

#include <WiFi101.h>
#include <WiFiUdp.h>

#undef min
#undef max
#undef abs

#include "common.h"

namespace fk {

class Wifi;

class UDPDiscovery {
private:
    WiFiUDP udp_;
    bool initialized_{ false };
    uint32_t publish_{ 0 };
    uint32_t receive_{ 0 };

public:
    UDPDiscovery();
    virtual ~UDPDiscovery();

public:
    bool start();
    bool service();
    void stop();

private:
    bool send();

};

}

#endif // defined(__SAMD51__)
