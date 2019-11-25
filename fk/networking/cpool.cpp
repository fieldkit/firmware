#include <fk-app-protocol.h>

#include "networking/cpool.h"
#include "networking/http_connection.h"
#include "networking/debug_connection.h"

namespace fk {

FK_DECLARE_LOGGER("httpd");

ConnectionPool::ConnectionPool(HttpRouter &router) : router_(&router) {
    activity_ = fk_uptime();
}

ConnectionPool::~ConnectionPool() {
    for (auto i = (size_t)0; i < MaximumConnections; ++i) {
        if (connections_[i] != nullptr) {
            update_statistics(connections_[i]);
            free_connection(i);
        }
    }
}

size_t ConnectionPool::available() {
    size_t used = 0;
    for (auto i = (size_t)0; i < MaximumConnections; ++i) {
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

    for (auto i = (size_t)0; i < MaximumConnections; ++i) {
        if (connections_[i] != nullptr) {
            auto c = connections_[i];

            if (log_status) {
                auto activity_elapsed = now - c->activity_;
                auto started_elapsed = now - c->started_;

                update_statistics(c);

                if (activity_elapsed < NetworkConnectionMaximumDuration) {
                    loginfo("[%" PRIu32 "] [%zd] active (%" PRIu32 "ms) (%" PRIu32 "ms) (%" PRIu32 " down) (%" PRIu32 " up)",
                            c->number_, i, activity_elapsed, started_elapsed, c->bytes_rx_, c->bytes_tx_);
                }
                else {
                    logwarn("[%" PRIu32 "] [%zd] killing (%" PRIu32 "ms) (%" PRIu32 "ms) (%" PRIu32 " down) (%" PRIu32 " up)",
                            c->number_, i, activity_elapsed, started_elapsed, c->bytes_rx_, c->bytes_tx_);
                    c->close();
                    free_connection(i);
                    continue;
                }
            }

            if (!c->service()) {
                // Do this before freeing to avoid a race empty pool after a
                // long connection, for example.
                update_statistics(c);
                activity_ = now;
                free_connection(i);
            }
        }
    }
}

void ConnectionPool::queue(PoolPointer<NetworkConnection> *c, Connection *connection) {
    for (auto i = (size_t)0; i < MaximumConnections; ++i) {
        if (connections_[i] == nullptr) {
            ip4_address ip{ c->get()->remote_address() };

            loginfo("[%" PRIu32 "] connection (socket = %" PRId32 ") (%d.%d.%d.%d)",
                    connection->number(), c->get()->socket(),
                    ip.u.bytes[0], ip.u.bytes[1], ip.u.bytes[2], ip.u.bytes[3]);

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
    // TODO This has races.

    auto rx = c->bytes_rx_ - c->bytes_rx_previous_;
    auto tx = c->bytes_tx_ - c->bytes_tx_previous_;

    c->bytes_rx_previous_ = c->bytes_rx_;
    c->bytes_tx_previous_ = c->bytes_tx_;

    bytes_rx_ += rx;
    bytes_tx_ += tx;
}

void ConnectionPool::free_connection(uint16_t index) {
    delete pools_[index];
    connections_[index] = nullptr;
    pools_[index] = nullptr;
}


DebugServerConnection::DebugServerConnection(Pool *pool, NetworkConnection *conn, uint32_t number) : Connection(pool, conn, number) {
}

DebugServerConnection::~DebugServerConnection() {
}

}
