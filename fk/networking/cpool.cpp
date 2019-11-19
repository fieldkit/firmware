#include <fk-app-protocol.h>

#include "networking/cpool.h"
#include "protobuf.h"
#include "base64.h"

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
                    loginfo("[%" PRIu32 "] [%zd] active (%" PRIu32 "ms) (%" PRIu32 "ms) (%" PRIu32 " down) (%" PRIu32 " up)", c->number_, i, activity_elapsed, started_elapsed, c->bytes_rx_, c->bytes_tx_);
                }
                else {
                    logwarn("[%" PRIu32 "] [%zd] killing (%" PRIu32 "ms) (%" PRIu32 "ms) (%" PRIu32 " down) (%" PRIu32 " up)", c->number_, i, activity_elapsed, started_elapsed, c->bytes_rx_, c->bytes_tx_);
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

            loginfo("[%" PRIu32 "] connection (%d.%d.%d.%d)", connection->number(), ip.u.bytes[0], ip.u.bytes[1], ip.u.bytes[2], ip.u.bytes[3]);

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

Connection::Connection(Pool *pool, NetworkConnection *conn, uint32_t number) : pool_{ pool }, conn_(conn), number_(number) {
    started_ = activity_ = fk_uptime();
}

Connection::~Connection() {
    close();
}

bool Connection::service() {
    if (conn_->status() != NetworkConnectionStatus::Connected) {
        loginfo("[%" PRIu32 "] disconnected", number_);
        return false;
    }

    return true;
}

int32_t Connection::read(uint8_t *buffer, size_t size) {
    auto bytes = conn_->read(buffer, size);
    if (bytes > 0) {
        bytes_rx_ += bytes;
        activity_ = fk_uptime();
    }

    return bytes;
}

int32_t Connection::write(uint8_t const *buffer, size_t size) {
    auto bytes = conn_->write(buffer, size);
    if (bytes > 0) {
        bytes_tx_ += bytes;
        activity_ = fk_uptime();
    }
    return bytes;
}

int32_t Connection::printf(const char *s, ...) {
    va_list args;
    va_start(args, s);
    auto r = conn_->vwritef(s, args);
    va_end(args);
    bytes_tx_ += r;
    return r;
}

int32_t Connection::close() {
    loginfo("[%" PRIu32 "] close", number_);
    conn_->stop();
    return 0;
}

HttpServerConnection::HttpServerConnection(Pool *pool, NetworkConnection *conn, uint32_t number, HttpRouter *router) : Connection(pool, conn, number), router_{ router }, req_{ pool_ }, buffer_{ nullptr }, size_{ 0 }, position_{ 0 } {
}

HttpServerConnection::~HttpServerConnection() {
}

int32_t HttpServerConnection::plain(int32_t status, const char *status_description, const char *text) {
    loginfo("[%" PRIu32 "] sending %" PRId32 " '%s'", number_, status, text);

    auto length = strlen(text);

    bytes_tx_ += conn_->writef("HTTP/1.1 %" PRId32 " %s\n", status, status_description);
    bytes_tx_ += conn_->writef("Content-Length: %zu\n", length);
    bytes_tx_ += conn_->write("Content-Type: text/plain\n");
    bytes_tx_ += conn_->write("Connection: close\n");
    bytes_tx_ += conn_->write("\n");
    bytes_tx_ += conn_->write(text);

    req_.finished();

    return 0;
}

int32_t HttpServerConnection::read(uint8_t *buffer, size_t size) {
    auto buffered_bytes = req_.read_buffered_body(buffer, size);
    if (buffered_bytes > 0) {
        bytes_rx_ += buffered_bytes;
        return buffered_bytes;
    }

    return Connection::read(buffer, size);
}

int32_t HttpServerConnection::fault() {
    return plain(500, "internal error", "");
}

int32_t HttpServerConnection::busy(uint32_t delay, const char *message) {
    fk_app_Error errors[] = {
        {
            .message = {
                .funcs = {},
                .arg = (void *)message,
            },
            .delay = delay,
        }
    };

    pb_array_t errors_array = {
        .length = sizeof(errors) / sizeof(fk_app_Error),
        .itemSize = sizeof(fk_app_Error),
        .buffer = pool_->copy(errors, sizeof(errors)),
        .fields = fk_app_Error_fields,
    };

    fk_app_HttpReply reply = fk_app_HttpReply_init_default;
    reply.type = fk_app_ReplyType_REPLY_BUSY;
    reply.errors.arg = (void *)pool_->copy(&errors_array, sizeof(errors_array));

    logwarn("[%" PRIu32 "] busy reply '%s'", number_, message);

    return write(503, message, &reply);
}

int32_t HttpServerConnection::error(const char *message) {
    fk_app_Error errors[] = {
        {
            .message = {
                .funcs = {},
                .arg = (void *)message,
            },
        }
    };

    pb_array_t errors_array = {
        .length = sizeof(errors) / sizeof(fk_app_Error),
        .itemSize = sizeof(fk_app_Error),
        .buffer = pool_->copy(errors, sizeof(errors)),
        .fields = fk_app_Error_fields,
    };

    fk_app_HttpReply reply = fk_app_HttpReply_init_default;
    reply.type = fk_app_ReplyType_REPLY_ERROR;
    reply.errors.arg = (void *)pool_->copy(&errors_array, sizeof(errors_array));

    logwarn("[%" PRIu32 "] error reply '%s'", number_, message);

    return write(500, message, &reply);
}

int32_t HttpServerConnection::write(fk_app_HttpReply const *reply) {
    return write(200, "OK", reply);
}

int32_t HttpServerConnection::write(int32_t statusCode, const char *message, fk_app_HttpReply const *reply) {
    auto started = fk_uptime();

    size_t size = 0;
    auto fields = fk_app_HttpReply_fields;
    if (!pb_get_encoded_size(&size, fields, reply)) {
        return fault();
    }

    size += pb_varint_size(size);

    auto content_size = hex_encoding_ ? size * 2 : size;

    logdebug("[%" PRIu32 "] replying (%zd bytes)", number_, content_size);

    bytes_tx_ += conn_->writef("HTTP/1.1 %" PRId32 " %s\n", statusCode, message);
    bytes_tx_ += conn_->writef("Content-Length: %zu\n", content_size);
    bytes_tx_ += conn_->writef("Content-Type: %s\n", "application/octet-stream");
    bytes_tx_ += conn_->write("Connection: close\n");
    bytes_tx_ += conn_->write("\n");

    logdebug("[%" PRIu32 "] headers done (%" PRIu32 "ms)", number_, fk_uptime() - started);

    BufferedWriter buffered{ this };
    Base64Writer b64_writer{ &buffered };
    Writer *writer = &buffered;

    if (hex_encoding_) {
        writer = &b64_writer;
    }

    auto ostream = pb_ostream_from_writable(writer);
    if (!pb_encode_delimited(&ostream, fields, reply)) {
        return content_size;
    }

    bytes_tx_ += content_size;
    activity_ = fk_uptime();

    req_.finished();

    logdebug("[%" PRIu32 "] done writing (%" PRIu32 "ms)", number_, fk_uptime() - started);

    return content_size;
}

bool HttpServerConnection::service() {
    if (!Connection::service()) {
        return false;
    }

    if (!req_.have_headers() && conn_->available()) {
        if (buffer_ == nullptr) {
            size_ = HttpConnectionBufferSize;
            buffer_ = (uint8_t *)pool_->malloc(HttpConnectionBufferSize);
            position_ = 0;
        }

        activity_ = fk_uptime();

        auto available = (size_ - 1) - position_;
        if (available > 0) {
            auto nread = read(buffer_ + position_, available);
            if (nread < 0) {
                loginfo("[%" PRIu32 "] error reading - eos", number_);
                return false;
            }

            if (nread == 0) {
                loginfo("[%" PRIu32 "] error reading - empty", number_);
                return false;
            }

            auto ptr = (char *)(buffer_ + position_);
            ptr[nread] = 0;
            auto parsed = req_.parse(ptr, nread);
            if (parsed == 0) {
                loginfo("[%" PRIu32 "] error parsing", number_);
                return false;
            }

            position_ += nread;
        }
    }

    if (req_.done()) {
        auto size = pool_->size();
        auto used = pool_->used();
        auto elapsed = fk_uptime() - started_;
        loginfo("[%" PRIu32 "] closing (%" PRIu32 " tx) (%" PRIu32 " rx) (%zd/%zd pooled) (%" PRIu32 "ms)", number_, bytes_tx_, bytes_rx_, used, size, elapsed);
        return false;
    }

    if (req_.have_headers()) {
        if (!routed_) {
            auto path = req_.url_parser().path();
            if (path == nullptr) {
                plain(404, "not found", "");
                return true;
            }

            loginfo("[%" PRIu32 "] routing '%s' path = '%s' (%" PRIu32 " bytes) ('%s')",
                    number_, req_.url(), path, req_.length(), req_.user_agent());

            auto handler = router_->route(path);
            if (handler == nullptr) {
                plain(404, "not found", "");
            }
            else {
                if (!handler->handle(this, *pool_)) {
                    plain(500, "internal error", "");
                }
            }
            routed_ = true;
            activity_ = fk_uptime();
        }
    }

    return true;
}

DebugServerConnection::DebugServerConnection(Pool *pool, NetworkConnection *conn, uint32_t number) : Connection(pool, conn, number) {
}

DebugServerConnection::~DebugServerConnection() {
}

}
