#include <fk-app-protocol.h>

#include "networking/cpool.h"
#include "protobuf.h"
#include "base64.h"

namespace fk {

FK_DECLARE_LOGGER("httpd");

ConnectionPool::ConnectionPool() {
    activity_ = fk_uptime();
}

ConnectionPool::~ConnectionPool() {
    for (auto i = (size_t)0; i < MaximumConnections; ++i) {
        if (pool_[i] != nullptr) {
            delete pool_[i];
            pool_[i] = nullptr;
        }
    }
}

size_t ConnectionPool::available() {
    size_t used = 0;
    for (auto i = (size_t)0; i < MaximumConnections; ++i) {
        if (pool_[i] != nullptr) {
            used++;
        }
    }
    return MaximumConnections - used;
}

void ConnectionPool::service(HttpRouter &router) {
    auto now = fk_uptime();
    auto log_status = false;
    if (now > status_) {
        status_ = now + OneSecondMs;
        log_status = true;
    }

    for (auto i = (size_t)0; i < MaximumConnections; ++i) {
        if (pool_[i] != nullptr) {
            auto c = pool_[i]->get();

            if (log_status) {
                auto elapsed = now - c->started_;
                if (elapsed < FiveSecondsMs) {
                    loginfo("[%" PRIu32 "] [%zd] active (%" PRIu32 "ms) (%" PRIu32 " bytes)", c->number_, i, elapsed, c->read_);
                }
                else {
                    logwarn("[%" PRIu32 "] [%zd] killing (%" PRIu32 "ms) (%" PRIu32 " bytes)", c->number_, i, elapsed, c->read_);
                    c->close();
                    delete pool_[i];
                    pool_[i] = nullptr;
                    continue;
                }
            }

            if (!c->service(router)) {
                // Do this before freeing to avoid a race empty pool after a
                // long connection, for example.
                activity_ = now;
                delete pool_[i];
                pool_[i] = nullptr;
            }
        }
    }
}

void ConnectionPool::queue(NetworkConnection *c) {
    for (auto i = (size_t)0; i < MaximumConnections; ++i) {
        if (pool_[i] == nullptr) {
            ip4_address ip{ c->remote_address() };

            auto number = counter_++;

            loginfo("[%" PRIu32 "] connection (%d.%d.%d.%d)", number, ip.u.bytes[0], ip.u.bytes[1], ip.u.bytes[2], ip.u.bytes[3]);

            activity_ = fk_uptime();
            pool_[i] = create_pool_wrapper<Connection, HttpdConnectionWorkSize>(c, number);
            return;
        }
    }
    FK_ASSERT(false);
}

Connection::Connection(Pool &pool, NetworkConnection *conn, uint32_t number) : pool_{ &pool }, conn_(conn), number_(number), req_{ pool_ }, buffer_{ nullptr }, size_{ 0 }, position_{ 0 } {
    started_ = fk_uptime();
}

Connection::~Connection() {
    close();
}

bool Connection::service(HttpRouter &router) {
    // TODO: 414 Request-URI Too Long
    // TODO: 431 Request Header Fields Too Large.
    // TODO: 413 Payload Too Large
    // TODO: 500 Service Unavailable
    // TODO: 503 Service Unavailable
    if (conn_->status() != NetworkConnectionStatus::Connected) {
        loginfo("[%" PRIu32 "] disconnected", number_);
        return false;
    }

    if (!req_.have_headers() && conn_->available()) {
        if (buffer_ == nullptr) {
            size_ = HttpdConnectionBufferSize;
            buffer_ = (uint8_t *)pool_->malloc(HttpdConnectionBufferSize);
            position_ = 0;
        }

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
            if (req_.parse(ptr, nread) != 0) {
                loginfo("[%" PRIu32 "] error parsing", number_);
                return false;
            }

            position_ += nread;
        }
    }

    if (req_.have_headers()) {
        if (!routed_) {
            loginfo("[%" PRIu32 "] routing '%s' (%" PRIu32 " bytes)", number_, req_.url(), req_.length());

            auto handler = router.route(req_.url());
            if (handler == nullptr) {
                plain(404, "not found", "");
            }
            else {
                if (!handler->handle(this, *pool_)) {
                    plain(500, "internal error", "");
                }
            }
            routed_ = true;
        }
    }

    if (req_.done()) {
        auto size = pool_->size();
        auto used = pool_->used();
        auto elapsed = fk_uptime() - started_;
        loginfo("[%" PRIu32 "] closing (%" PRIu32 " up) (%" PRIu32 " down) (%zd/%zd pooled) (%" PRIu32 "ms)", number_, wrote_, read_, used, size, elapsed);
        return false;
    }

    return true;
}

int32_t Connection::read(uint8_t *buffer, size_t size) {
    auto buffered_bytes = req_.read_buffered_body(buffer, size);
    if (buffered_bytes > 0) {
        read_ += buffered_bytes;
        return buffered_bytes;
    }

    auto bytes = conn_->read(buffer, size);
    if (bytes > 0) {
        read_ += bytes;
    }

    return bytes;
}

int32_t Connection::write(uint8_t const *buffer, size_t size) {
    auto bytes = conn_->write(buffer, size);
    if (wrote_ > 0) {
        wrote_ += bytes;
    }
    return bytes;
}

int32_t Connection::write(fk_app_HttpReply const *reply) {
    auto started = fk_uptime();

    size_t size = 0;
    auto fields = fk_app_HttpReply_fields;
    if (!pb_get_encoded_size(&size, fields, reply)) {
        return fault();
    }

    size += pb_varint_size(size);

    auto content_size = hex_encoding_ ? size * 2 : size;

    logdebug("[%" PRIu32 "] replying (%zd bytes)", number_, content_size);

    wrote_ += conn_->write("HTTP/1.1 200 OK\n");
    wrote_ += conn_->writef("Content-Length: %zu\n", content_size);
    wrote_ += conn_->writef("Content-Type: %s\n", "application/octet-stream");
    wrote_ += conn_->write("Connection: close\n");
    wrote_ += conn_->write("\n");

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

    wrote_ += content_size;

    req_.finished();

    logdebug("[%" PRIu32 "] done writing (%" PRIu32 "ms)", number_, fk_uptime() - started);

    return content_size;
}

int32_t Connection::printf(const char *s, ...) {
    va_list args;
    va_start(args, s);
    auto r = conn_->vwritef(s, args);
    va_end(args);
    wrote_ += r;
    return r;
}

int32_t Connection::plain(int32_t status, const char *status_description, const char *text) {
    loginfo("[%" PRIu32 "] sending %" PRId32 " '%s'", number_, status, text);

    auto length = strlen(text);

    wrote_ += conn_->writef("HTTP/1.1 %" PRId32 " %s\n", status, status_description);
    wrote_ += conn_->writef("Content-Length: %zu\n", length);
    wrote_ += conn_->write("Content-Type: text/plain\n");
    wrote_ += conn_->write("Connection: close\n");
    wrote_ += conn_->write("\n");
    wrote_ += conn_->write(text);

    req_.finished();

    return 0;
}

int32_t Connection::fault() {
    return plain(500, "internal error", "");
}

int32_t Connection::busy(const char *message) {
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
    reply.type = fk_app_ReplyType_REPLY_BUSY;
    reply.errors.arg = (void *)pool_->copy(&errors_array, sizeof(errors_array));

    logwarn("[%" PRIu32 "] busy reply '%s'", number_, message);

    return write(&reply);
}

int32_t Connection::error(const char *message) {
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

    return write(&reply);
}

int32_t Connection::close() {
    loginfo("[%" PRIu32 "] close", number_);
    conn_->stop();
    return 0;
}

}
