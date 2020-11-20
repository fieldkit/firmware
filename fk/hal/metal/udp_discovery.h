#pragma once

#if defined(__SAMD51__)

#include <WiFi101.h>
#include <WiFiUdp.h>

#undef min
#undef max
#undef abs

#include "common.h"
#include "networking/debug_udp.h"
#include "records.h"

namespace fk {

class UDPDiscovery {
private:
    Pool *pool_{ nullptr };
    WiFiUDP wifi_udp_;
    DebugUDP udp_{ wifi_udp_, "sud" };
    bool initialized_{ false };
    uint32_t publish_{ 0 };
    uint32_t receive_{ 0 };

public:
    UDPDiscovery();
    virtual ~UDPDiscovery();

public:
    void pool(Pool *pool) {
        pool_ = pool;
        udp_.pool(pool);
    }

public:
    bool start();
    bool service(Pool *pool);
    void stop();

public:
    bool send(fk_app_UdpStatus status, Pool *pool);

};

}

#endif // defined(__SAMD51__)
