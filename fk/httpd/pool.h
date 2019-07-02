#pragma once

#include "hal/hal.h"
#include "httpd.h"

namespace fk {

class ConnectionPool {
private:
    typedef struct Connection {
        WifiConnection *c{ nullptr };
        StaticPool<128> pool{ "conn" };
        HttpRequest req{ &pool };
        uint8_t data[1024];
        size_t position;
    } Connection;

    constexpr static size_t MaximumConnections = 4;
    Connection pool_[MaximumConnections];

public:
    size_t available();

    void service();

    void queue(WifiConnection *c);

private:
    bool service(Connection *c);

};

}
