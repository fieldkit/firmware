#include "networking/connection.h"

#include "hal/hal.h"

namespace fk {

FK_DECLARE_LOGGER("connection");

Connection::Connection(Pool *pool, NetworkConnection *conn, uint32_t number) : pool_{ pool }, conn_(conn), number_(number) {
    started_ = activity_ = fk_uptime();
}

Connection::~Connection() {
    close();
}

bool Connection::service() {
    // See ::close
    if (conn_ == nullptr) {
        logwarn("[%" PRIu32 "] !conn_", number_);
        return true;
    }

    if (FK_ADDRESS_VALID(conn_)) {
        auto status = wifi_mutex.with<NetworkConnectionStatus>([&]() {
            return conn_->status();
        });
        if (status != NetworkConnectionStatus::Connected) {
            loginfo("[%" PRIu32 "] disconnected", number_);
            return false;
        }
    }
    else {
        FK_ASSERT_ADDRESS(conn_);
    }

    return true;
}

int32_t Connection::read(uint8_t *buffer, size_t size) {
    // See ::close
    if (conn_ == nullptr) {
        logwarn("[%" PRIu32 "] !conn_", number_);
        return -1;
    }

    auto bytes = wifi_mutex.with<int32_t>([&]() {
        return conn_->read(buffer, size);
    });
    if (bytes > 0) {
        bytes_rx_ += bytes;
        activity_ = fk_uptime();
    }

    return bytes;
}

int32_t Connection::write(uint8_t const *buffer, size_t size) {
    // See ::close
    if (conn_ == nullptr) {
        logwarn("[%" PRIu32 "] !conn_", number_);
        return -1;
    }

    auto bytes = wifi_mutex.with<int32_t>([&]() {
        return conn_->write(buffer, size);
    });
    if (bytes > 0) {
        bytes_tx_ += bytes;
        activity_ = fk_uptime();
    }
    return bytes;
}

int32_t Connection::printf(const char *s, ...) {
    // See ::close
    if (conn_ == nullptr) {
        logwarn("[%" PRIu32 "] !conn_", number_);
        return -1;
    }

    va_list args;
    va_start(args, s);
    auto r = wifi_mutex.with<int32_t>([&]() {
        return conn_->vwritef(s, args);
    });
    va_end(args);
    bytes_tx_ += r;
    return r;
}

int32_t Connection::close() {
    // This can happen in a separate task than the one calling
    // ::service and introduce a race, and so we check for nullptr's
    // in all the public methods.
    loginfo("[%" PRIu32 "] close", number_);
    return wifi_mutex.with<int32_t>([=]() {
        if (conn_ != nullptr) {
            auto c = conn_;
            conn_ = nullptr;
            c->stop();
        }
        return 0;
    });
}

}
