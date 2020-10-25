#pragma once

#if defined(__SAMD51__)

#include <WiFi101.h>
#include <WiFiUdp.h>
#include <ArduinoMDNS.h>
#include "hal/metal/debug_udp.h"
#include "common.h"
#include "pool.h"

namespace fk {

#undef min
#undef max
#undef abs

class MDNSDiscovery {
private:
    Pool *pool_;
    DebugUDP udp_{ "mdns" };
    MDNS mdns_{ udp_ };
    bool initialized_{ false };
    uint32_t registered_{ 0 };
    uint32_t publish_{ 0 };
    char service_name_[64];
    char name_[64];

public:
    MDNSDiscovery();
    virtual ~MDNSDiscovery();

public:
    void pool(Pool *pool) {
        pool_ = pool;
        udp_.pool(pool);
    }

public:
    bool start();
    bool service(Pool *pool);
    void stop();

};

}

#endif
