#include <fk-app-protocol.h>

#include "httpd/cpool.h"
#include "protobuf.h"

namespace fk {

#define loginfo(f, ...)  loginfof("httpd", f, ##__VA_ARGS__)

#define logerror(f, ...) logerrorf("httpd", f, ##__VA_ARGS__)

static bool write_callback(pb_ostream_t *stream, const uint8_t *buf, size_t c) {
    auto nc = reinterpret_cast<NetworkConnection*>(stream->state);
    return nc->write(buf, c) == (int32_t)c;
}

static bool read_callback(pb_istream_t *stream, uint8_t *buf, size_t c) {
    auto nc = reinterpret_cast<NetworkConnection*>(stream->state);
    auto nread = nc->read(buf, c);
    if (nread <= 0) {
        stream->bytes_left = 0; /* EOF */
    }
    return nread == (int32_t)c;
}

pb_ostream_t pb_ostream_from_connection(NetworkConnection *nc) {
    pb_ostream_t stream = { &write_callback, (void *)nc, SIZE_MAX, 0 };
    return stream;
}

pb_istream_t pb_istream_from_connection(NetworkConnection *nc) {
    pb_istream_t stream = { &read_callback, (void *)nc, SIZE_MAX };
    return stream;
}

ConnectionPool::ConnectionPool() : memory_("httpd", 0 * HttpdConnectionWorkSize * MaximumConnections) {
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

void ConnectionPool::service() {
    for (auto i = (size_t)0; i < MaximumConnections; ++i) {
        if (pool_[i] != nullptr) {
            if (!pool_[i]->service()) {
                delete pool_[i];
                pool_[i] = nullptr;
            }
        }
    }
}

void ConnectionPool::queue(NetworkConnection *c) {
    for (auto i = (size_t)0; i < MaximumConnections; ++i) {
        if (pool_[i] == nullptr) {
            pool_[i] = new Connection(c, HttpdConnectionWorkSize);
            return;
        }
    }
    FK_ASSERT(false);
}

Connection::Connection(NetworkConnection *conn, size_t size) : conn_(conn), pool_{ "conn", size }, req_{ &pool_ }, buffer_{ nullptr }, size_{ 0 }, position_{ 0 } {
    started_ = fk_uptime();
}

Connection::~Connection() {
    conn_->stop();
}

bool Connection::service() {
    // TODO: 414 Request-URI Too Long
    // TODO: 431 Request Header Fields Too Large.
    // TODO: 413 Payload Too Large
    // TODO: 500 Service Unavailable
    // TODO: 503 Service Unavailable
    if (conn_->status() != NetworkConnectionStatus::Connected) {
        loginfo("disconnected");
        return false;
    }

    if (conn_->available()) {
        if (buffer_ == nullptr) {
            size_ = HttpdConnectionBufferSize;
            buffer_ = (uint8_t *)pool_.malloc(HttpdConnectionBufferSize);
            position_ = 0;
        }

        auto available = (size_ - 1) - position_;
        auto nread = conn_->read(buffer_ + position_, available);
        if (nread < 0) {
            loginfo("error reading - eos");
            return false;
        }

        if (nread == 0) {
            loginfo("error reading - empty");
            return false;
        }

        auto ptr = (char *)(buffer_ + position_);
        ptr[nread] = 0;
        if (req_.parse(ptr, nread) != 0) {
            loginfo("error parsing");
            return false;
        }

        auto query = req_.query();
        if (query != nullptr) {
            switch (query->type) {
            default: {
                break;
            }
            }
        }

        position_ += nread;
    }

    if (req_.have_headers()) {
        loginfo("routing '%s' (%" PRIu32 " bytes)", req_.url(), req_.length());
    }

    if (req_.consumed()) {
        auto elapsed = fk_uptime() - started_;
        loginfo("replying/closing (%" PRIu32 "ms)", elapsed);
        busy("Busy");
        return false;
    }

    return true;
}

int32_t Connection::write(fk_app_WireMessageReply *reply) {
    size_t size = 0;
    auto fields = fk_app_WireMessageQuery_fields;
    auto serialized = pool_.encode(fields, reply, &size);
    if (serialized == nullptr) {
        return fault();
    }

    conn_->write("HTTP/1.1 200 OK\n");
    conn_->writef("Content-Length: %zu\n", size);
    conn_->write("Content-Type: application/octet-stream\n");
    conn_->write("Connection: close\n");
    conn_->write("\n");
    conn_->write(serialized, size);
    return size;
}

int32_t Connection::plain(const char *text) {
    conn_->write("HTTP/1.1 200 OK\n");
    conn_->writef("Content-Length: %zu\n", strlen(text));
    conn_->write("Content-Type: text/plain\n");
    conn_->write("Connection: close\n");
    conn_->write("\n");
    conn_->write(text);
    return 0;
}

int32_t Connection::fault() {
    conn_->write("HTTP/1.1 500 Internal Server Error\n");
    conn_->write("Content-Length: 0\n");
    conn_->write("Content-Type: text/plain\n");
    conn_->write("Connection: close\n");
    conn_->write("\n");
    return 0;
}

int32_t Connection::busy(const char *message) {
    fk_app_Error errors[] = {
        {
            .message = {
                .funcs = {
                    .encode = pb_encode_string,
                },
                .arg = (void *)message,
            },
        }
    };

    pb_array_t errors_array = {
        .length = sizeof(errors) / sizeof(fk_app_Error),
        .itemSize = sizeof(fk_app_Error),
        .buffer = pool_.copy(errors, sizeof(errors)),
        .fields = fk_app_Error_fields,
    };

    fk_app_WireMessageReply reply = fk_app_WireMessageReply_init_default;
    reply.type = fk_app_ReplyType_REPLY_BUSY;
    reply.errors.funcs.encode = pb_encode_array;
    reply.errors.arg = (void *)pool_.copy(&errors_array, sizeof(errors_array));

    return write(&reply);
}

int32_t Connection::error(const char *message) {
    fk_app_Error errors[] = {
        {
            .message = {
                .funcs = {
                    .encode = pb_encode_string,
                },
                .arg = (void *)message,
            },
        }
    };

    pb_array_t errors_array = {
        .length = sizeof(errors) / sizeof(fk_app_Error),
        .itemSize = sizeof(fk_app_Error),
        .buffer = pool_.copy(errors, sizeof(errors)),
        .fields = fk_app_Error_fields,
    };

    fk_app_WireMessageReply reply = fk_app_WireMessageReply_init_default;
    reply.type = fk_app_ReplyType_REPLY_ERROR;
    reply.errors.funcs.encode = pb_encode_array;
    reply.errors.arg = (void *)pool_.copy(&errors_array, sizeof(errors_array));

    return write(&reply);
}

}
