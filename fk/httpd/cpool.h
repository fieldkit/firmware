#pragma once

#include "common.h"
#include "pool.h"
#include "hal/hal.h"
#include "httpd/req.h"
#include "httpd/routing.h"

namespace fk {

constexpr static size_t HttpdConnectionBufferSize = 1024;
constexpr static size_t HttpdConnectionWorkSize = 2048;

class Connection {
private:
    NetworkConnection *conn_{ nullptr };
    MallocPool pool_;
    HttpRequest req_;
    uint8_t *buffer_;
    size_t size_;
    size_t position_;
    uint32_t started_{ 0 };

public:
    Connection(NetworkConnection *conn, size_t size);
    virtual ~Connection();

public:
    bool service(HttpRouter &router);

    int32_t write(fk_app_WireMessageReply *reply);

    int32_t plain(int32_t status, const char *status_description, const char *text);

    int32_t busy(const char *message);

    int32_t error(const char *message);

    int32_t fault();

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

    void service(HttpRouter &router);

    void queue(NetworkConnection *c);

};

}
