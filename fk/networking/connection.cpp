#include "networking/connection.h"

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

    FK_ASSERT_ADDRESS(conn_);

    if (conn_->status() != NetworkConnectionStatus::Connected) {
        loginfo("[%" PRIu32 "] disconnected", number_);
        return false;
    }

    return true;
}

int32_t Connection::read(uint8_t *buffer, size_t size) {
    // See ::close
    if (conn_ == nullptr) {
        logwarn("[%" PRIu32 "] !conn_", number_);
        return -1;
    }

    auto bytes = conn_->read(buffer, size);
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

    auto bytes = conn_->write(buffer, size);
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
    auto r = conn_->vwritef(s, args);
    va_end(args);
    bytes_tx_ += r;
    return r;
}

int32_t Connection::close() {
    // This can happen in a separate task than the one calling
    // ::service and introduce a race, and so we check for nullptr's
    // in all the public methods.
    if (conn_ != nullptr) {
        loginfo("[%" PRIu32 "] close", number_);
        conn_->stop();
        conn_ = nullptr;
    }
    return 0;
}

}
