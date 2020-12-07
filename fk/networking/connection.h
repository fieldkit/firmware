#pragma once

#include "io.h"
#include "pool.h"
#include "hal/network.h"
#include "platform.h"

namespace fk {

class Connection : public Writer, public Reader {
protected:
    Pool *pool_;
    NetworkConnection *conn_;
    uint32_t number_{ 0 };
    uint32_t started_{ 0 };
    uint32_t activity_{ 0 };
    uint32_t bytes_rx_{ 0 };
    uint32_t bytes_tx_{ 0 };
    uint32_t bytes_rx_previous_{ 0 };
    uint32_t bytes_tx_previous_{ 0 };

public:
    Connection(Pool *pool, NetworkConnection *conn, uint32_t number);
    virtual ~Connection();

    friend class ConnectionPool;

public:
    virtual bool service();

    int32_t printf(const char *s, ...) __attribute__((format(printf, 2, 3)));

    int32_t write(uint8_t const *buffer, size_t size) override;

    int32_t read(uint8_t *buffer, size_t size) override;

    int32_t close();

public:
    uint32_t number() const {
        return number_;
    }

    uint32_t activity() const {
        return activity_;
    }

    bool active() const {
        return (fk_uptime() - activity()) < NetworkConnectionMaximumDuration;
    }

    bool closed() const {
        if (conn_ != nullptr) {
            // alogf(LogLevels::DEBUG, "debugging", "[0x%p] connection: 0x%p", this, conn_);
            FK_ASSERT_ADDRESS(conn_);
            return false;
        }
        // alogf(LogLevels::DEBUG, "debugging", "[0x%p] closed", this);
        return true;
    }

    Pool *pool() {
        return pool_;
    }

};

}
