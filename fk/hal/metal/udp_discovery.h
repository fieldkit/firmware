#pragma once

#if defined(__SAMD51__)

#include <WiFi101.h>
#include <WiFiUdp.h>

#undef min
#undef max
#undef abs

#include "common.h"
#include "debug_udp.h"

namespace fk {

class Wifi;

class UDPDiscovery {
private:
    DebugUDP udp_{ "sud" };
    bool initialized_{ false };
    uint32_t publish_{ 0 };
    uint32_t receive_{ 0 };

public:
    UDPDiscovery();
    virtual ~UDPDiscovery();

public:
    void pool(Pool *pool) {
        udp_.pool(pool);
    }

public:
    bool start();
    bool service(Pool *pool);
    void stop();

private:
    bool send();

};

}

#endif // defined(__SAMD51__)
