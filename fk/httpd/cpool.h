#pragma once

#include "hal/hal.h"
#include "httpd.h"

namespace fk {

constexpr static size_t HttpdConnectionBufferSize = 1024;
constexpr static size_t HttpdConnectionWorkSize = 2048;

class Connection {
private:
    WifiConnection *conn_{ nullptr };
    MallocPool pool_;
    HttpRequest req_{ &pool_ };
    uint8_t *buffer_;
    size_t size_;
    size_t position_;

public:
    Connection(WifiConnection *conn, size_t size);
    virtual ~Connection();

public:
    bool service();

};

class ConnectionPool {
private:
    constexpr static size_t MaximumConnections = 4;

private:
    MallocPool memory_;
    Connection *pool_[MaximumConnections] = { nullptr };

public:
    ConnectionPool();

public:
    size_t available();

    void service();

    void queue(WifiConnection *c);

};

}
