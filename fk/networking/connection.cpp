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
    FK_ASSERT(conn_ != nullptr);

    FK_ASSERT_ADDRESS(conn_);

    if (conn_->status() != NetworkConnectionStatus::Connected) {
        loginfo("[%" PRIu32 "] disconnected", number_);
        return false;
    }

    return true;
}

int32_t Connection::read(uint8_t *buffer, size_t size) {
    FK_ASSERT(conn_ != nullptr);

    auto bytes = conn_->read(buffer, size);
    if (bytes > 0) {
        bytes_rx_ += bytes;
        activity_ = fk_uptime();
    }

    return bytes;
}

int32_t Connection::write(uint8_t const *buffer, size_t size) {
    FK_ASSERT(conn_ != nullptr);

    auto bytes = conn_->write(buffer, size);
    if (bytes > 0) {
        bytes_tx_ += bytes;
        activity_ = fk_uptime();
    }
    return bytes;
}

int32_t Connection::printf(const char *s, ...) {
    FK_ASSERT(conn_ != nullptr);

    va_list args;
    va_start(args, s);
    auto r = conn_->vwritef(s, args);
    va_end(args);
    bytes_tx_ += r;
    return r;
}

int32_t Connection::close() {
    if (conn_ != nullptr) {
        loginfo("[%" PRIu32 "] close", number_);
        conn_->stop();
        conn_ = nullptr;
    }
    return 0;
}

}
