#include <fk-app-protocol.h>

#include "networking/cpool.h"
#include "networking/http_server_connection.h"
#include "networking/debug_connection.h"

namespace fk {

FK_DECLARE_LOGGER("httpd");

ConnectionPool::ConnectionPool(HttpRouter &router) : router_(&router) {
    activity_ = fk_uptime();
}

ConnectionPool::~ConnectionPool() {
    for (auto i = 0u; i < MaximumConnections; ++i) {
        if (connections_[i] != nullptr) {
            update_statistics(connections_[i]);
            free_connection(i);
        }
    }
}

size_t ConnectionPool::available() {
    auto used = 0;
    for (auto i = 0u; i < MaximumConnections; ++i) {
        if (connections_[i] != nullptr) {
            used++;
        }
    }
    return MaximumConnections - used;
}

void ConnectionPool::service() {
    auto now = fk_uptime();
    auto log_status = false;
    if (now > status_) {
        status_ = now + OneSecondMs;
        log_status = true;
    }

    for (auto i = 0u; i < MaximumConnections; ++i) {
        if (connections_[i] != nullptr) {
            auto c = connections_[i];

            if (log_status) {
                auto activity_elapsed = now - c->activity_;
                auto started_elapsed = now - c->started_;

                update_statistics(c);

                if (activity_elapsed < NetworkConnectionMaximumDuration) {
                    logtrace("[%" PRIu32 "] [%d] active (%" PRIu32 "ms) (%" PRIu32 "ms) (%" PRIu32 " down) (%" PRIu32 " up)", c->number(),
                             i, activity_elapsed, started_elapsed, c->bytes_rx_, c->bytes_tx_);
                } else {
                    logwarn("[%" PRIu32 "] [%d] killing (%" PRIu32 "ms) (%" PRIu32 "ms) (%" PRIu32 " down) (%" PRIu32 " up)", c->number(),
                            i, activity_elapsed, started_elapsed, c->bytes_rx_, c->bytes_tx_);
                    free_connection(i);
                    continue;
                }
            }

            auto closing = c->closed() || !c->service();
            if (closing && !c->busy()) {
                loginfo("[%d] closing: 0x%p", i, c);
                // Do this before freeing to avoid a race empty pool after a
                // long connection, for example.
                update_statistics(c);
                activity_ = now;
                free_connection(i);
            }
        }
    }
}

void ConnectionPool::stop() {
    for (auto i = 0u; i < MaximumConnections; ++i) {
        if (connections_[i] != nullptr) {
            free_connection(i);
        }
    }
}

void ConnectionPool::queue(PoolPointer<NetworkConnection> *c, Connection *connection) {
    for (auto i = 0u; i < MaximumConnections; ++i) {
        if (connections_[i] == nullptr) {
            ip4_address ip{ c->get()->remote_address() };

            loginfo("[%" PRIu32 "] connection (%d.%d.%d.%d)", connection->number(), ip.u.bytes[0], ip.u.bytes[1], ip.u.bytes[2],
                    ip.u.bytes[3]);

            activity_ = fk_uptime();

            pools_[i] = c;
            connections_[i] = connection;
            return;
        }
    }

    logerror("unable to queue connection!");

    delete c;
}

void ConnectionPool::queue_debug(PoolPointer<NetworkConnection> *c) {
    auto connection = new (c->pool()) DebugServerConnection(c->pool(), c->get(), counter_++);
    queue(c, connection);
}

void ConnectionPool::queue_http(PoolPointer<NetworkConnection> *c) {
    auto connection = new (c->pool()) HttpServerConnection(c->pool(), c->get(), counter_++, router_);
    queue(c, connection);
}

void ConnectionPool::update_statistics(Connection *c) {
    auto rx = c->bytes_rx_ - c->bytes_rx_previous_;
    auto tx = c->bytes_tx_ - c->bytes_tx_previous_;

    c->bytes_rx_previous_ = c->bytes_rx_;
    c->bytes_tx_previous_ = c->bytes_tx_;

    bytes_rx_ += rx;
    bytes_tx_ += tx;

    // Use connection's activity time, this makes us stay running if
    // we've got a debug connection.
    if (c->activity_ > activity_) {
        activity_ = c->activity_;
    }
}

void ConnectionPool::free_connection(uint16_t index) {
    auto connection = connections_[index];

    if (!connection->busy()) {
        auto pool = pools_[index];
        auto number = connection->number();

        connections_[index] = nullptr;
        pools_[index] = nullptr;

        logdebug("[%" PRIu32 "] [%d] free connection", number, index);
        connection->close();
        delete pool;
        logdebug("[%" PRIu32 "] [%d] connection freed", number, index);
    }
}

bool ConnectionPool::active_connections() const {
    for (auto i = 0u; i < MaximumConnections; ++i) {
        if (connections_[i] != nullptr) {
            return true;
        }
    }
    return false;
}

uint32_t ConnectionPool::activity() const {
    return activity_;
}

uint32_t ConnectionPool::bytes_rx() const {
    return bytes_rx_;
};

uint32_t ConnectionPool::bytes_tx() const {
    return bytes_tx_;
};

} // namespace fk
