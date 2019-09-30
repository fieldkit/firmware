#pragma once

#include "common.h"
#include "pool.h"
#include "hal/hal.h"
#include "networking/req.h"
#include "networking/routing.h"
#include "io.h"

namespace fk {

class Connection : public Writer, public Reader {
private:
    Pool *pool_;
    NetworkConnection *conn_;
    uint32_t number_{ 0 };
    HttpRequest req_;
    uint8_t *buffer_;
    size_t size_;
    size_t position_;
    uint32_t started_{ 0 };
    uint32_t activity_{ 0 };
    uint32_t bytes_rx_{ 0 };
    uint32_t bytes_tx_{ 0 };
    uint32_t bytes_rx_previous_{ 0 };
    uint32_t bytes_tx_previous_{ 0 };
    bool routed_{ false };
    bool hex_encoding_{ false };

public:
    Connection(Pool &pool, NetworkConnection *conn, uint32_t number);
    virtual ~Connection();

    friend class ConnectionPool;

public:
    void hex_encoding(bool hex_encoding) {
        hex_encoding_ = hex_encoding;
    }

    bool service(HttpRouter &router);

    int32_t printf(const char *s, ...) __attribute__((format(printf, 2, 3)));

    int32_t write(int32_t statusCode, const char *message, fk_app_HttpReply const *reply);

    int32_t write(fk_app_HttpReply const *reply);

    int32_t write(uint8_t const *buffer, size_t size) override;

    int32_t read(uint8_t *buffer, size_t size) override;

    int32_t plain(int32_t status, const char *status_description, const char *text);

    int32_t busy(uint32_t retry, const char *message);

    int32_t error(const char *message);

    int32_t fault();

    int32_t close();

public:
    WellKnownContentType content_type() const {
        return req_.content_type();
    }

    uint32_t length() const {
        return req_.length();
    }

    const char *find_query_param(const char *key, Pool &pool) {
        return req_.url_parser().find_query_param(key, pool);
    }

};

class ConnectionPool {
private:
    constexpr static size_t MaximumConnections = 4;

private:
    PoolWrapper<Connection> *pool_[MaximumConnections] = { nullptr };
    uint32_t activity_{ 0 };
    uint32_t status_{ 0 };
    uint32_t counter_{ 0 };
    uint32_t bytes_rx_{ 0 };
    uint32_t bytes_tx_{ 0 };

public:
    ConnectionPool();
    virtual ~ConnectionPool();

public:
    size_t available();

    void service(HttpRouter &router);

    void queue(NetworkConnection *c);

    uint32_t activity() const {
        return activity_;
    }

    bool active_connections() const {
        for (size_t i = 0; i < MaximumConnections; ++i) {
            if (pool_[i] != nullptr) {
                return true;
            }
        }
        return false;
    }

    uint32_t bytes_rx() const {
        return bytes_rx_;
    };

    uint32_t bytes_tx() const {
        return bytes_tx_;
    };

private:
    void update_statistics(Connection *c);

};

}
