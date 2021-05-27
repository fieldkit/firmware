#pragma once

#include "common.h"
#include "pool.h"
#include "hal/hal.h"
#include "networking/req.h"
#include "networking/routing.h"
#include "networking/connection.h"

namespace fk {

class ConnectionPool {
private:
    constexpr static size_t MaximumConnections = 4;

private:
    HttpRouter *router_;
    PoolPointer<NetworkConnection> *pools_[MaximumConnections] = { nullptr };
    Connection *connections_[MaximumConnections] = { nullptr };
    uint32_t activity_{ 0 };
    uint32_t status_{ 0 };
    uint32_t counter_{ 0 };
    uint32_t bytes_rx_{ 0 };
    uint32_t bytes_tx_{ 0 };

public:
    ConnectionPool(HttpRouter &router);
    virtual ~ConnectionPool();

public:
    size_t available();

    void service();

    void queue_debug(PoolPointer<NetworkConnection> *c);

    void queue_http(PoolPointer<NetworkConnection> *c);

    void queue(PoolPointer<NetworkConnection> *c, Connection *connection);

    bool active_connections() const;

    uint32_t activity() const;

    uint32_t bytes_rx() const;

    uint32_t bytes_tx() const;

private:
    void update_statistics(Connection *c);
    void free_connection(uint16_t index);

};

}
