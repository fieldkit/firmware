#include <fk-app-protocol.h>

#include "networking/cpool.h"
#include "protobuf.h"

namespace fk {

FK_DECLARE_LOGGER("httpd");

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
    for (auto i = (size_t)0; i < MaximumConnections; ++i) {
        if (pool_[i] != nullptr) {
            if (!pool_[i]->service(router)) {
                // Do this before freeing to avoid a race empty pool after a
                // long connection, for example.
                activity_ = fk_uptime();
                delete pool_[i];
                pool_[i] = nullptr;
            }
        }
    }
}

void ConnectionPool::queue(NetworkConnection *c) {
    for (auto i = (size_t)0; i < MaximumConnections; ++i) {
        if (pool_[i] == nullptr) {
            activity_ = fk_uptime();
            // TODO: MALLOC
            pool_[i] = new Connection(c, HttpdConnectionWorkSize);
            return;
        }
    }
    FK_ASSERT(false);
}

Connection::Connection(NetworkConnection *conn, size_t size) : conn_(conn), pool_{ "conn", size }, req_{ this, &pool_ }, buffer_{ nullptr }, size_{ 0 }, position_{ 0 } {
    started_ = fk_uptime();
}

Connection::~Connection() {
    conn_->stop();
}

bool Connection::service(HttpRouter &router) {
    // TODO: 414 Request-URI Too Long
    // TODO: 431 Request Header Fields Too Large.
    // TODO: 413 Payload Too Large
    // TODO: 500 Service Unavailable
    // TODO: 503 Service Unavailable
    if (conn_->status() != NetworkConnectionStatus::Connected) {
        loginfo("disconnected");
        return false;
    }

    // FK_ASSERT(fk_uptime() - started_ < 1000);

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

        position_ += nread;
    }

    if (!routed_) {
        if (req_.have_headers()) {
            loginfo("routing '%s' (%" PRIu32 " bytes)", req_.url(), req_.length());

            auto handler = router.route(req_.url());
            if (handler == nullptr) {
                plain(404, "not found", "");
            }
            else {
                if (!handler->handle(req_)) {
                    plain(500, "internal error", "");
                }
            }
            routed_ = true;
        }
    }

    if (req_.done()) {
        auto size = pool_.size();
        auto used = pool_.used();
        auto elapsed = fk_uptime() - started_;
        loginfo("closing (%d bytes) (%d/%d pooled) (%" PRIu32 "ms)", wrote_, used, size, elapsed);
        return false;
    }

    return true;
}

static void initialize_callbacks(fk_app_HttpReply *reply) {
    reply->errors.funcs.encode = pb_encode_array;
    if (reply->errors.arg != nullptr) {
        auto array = (pb_array_t *)reply->errors.arg;
        for (size_t i = 0; i < array->length; ++i) {
            fk_app_Error *error = &((fk_app_Error *)array->buffer)[i];
            error->message.funcs.encode = pb_encode_string;
        }
    }

    reply->status.identity.device.funcs.encode = pb_encode_string;
    reply->status.identity.stream.funcs.encode = pb_encode_string;
    reply->status.identity.deviceId.funcs.encode = pb_encode_data;
    reply->status.identity.firmware.funcs.encode = pb_encode_string;
    reply->status.identity.build.funcs.encode = pb_encode_string;
    reply->modules.funcs.encode = pb_encode_array;
    if (reply->modules.arg != nullptr) {
        auto array = (pb_array_t *)reply->modules.arg;
        for (size_t i = 0; i < array->length; ++i) {
            fk_app_ModuleCapabilities *module = &((fk_app_ModuleCapabilities *)array->buffer)[i];
            module->name.funcs.encode = pb_encode_string;
            module->path.funcs.encode = pb_encode_string;
            module->sensors.funcs.encode = pb_encode_array;
            if (module->sensors.arg != nullptr) {
                auto array = (pb_array_t *)module->sensors.arg;
                for (size_t i = 0; i < array->length; ++i) {
                    fk_app_SensorCapabilities *module = &((fk_app_SensorCapabilities *)array->buffer)[i];
                    module->name.funcs.encode = pb_encode_string;
                    module->path.funcs.encode = pb_encode_string;
                    module->unitOfMeasure.funcs.encode = pb_encode_string;
                }
            }
        }
    }

    reply->streams.funcs.encode = pb_encode_array;
    if (reply->streams.arg != nullptr) {
        auto array = (pb_array_t *)reply->streams.arg;
        for (size_t i = 0; i < array->length; ++i) {
            fk_app_DataStream *stream = &((fk_app_DataStream *)array->buffer)[i];
            stream->name.funcs.encode = pb_encode_string;
            stream->path.funcs.encode = pb_encode_string;
            stream->hash.funcs.encode = pb_encode_data;
        }
    }

    reply->networkSettings.networks.funcs.encode = pb_encode_array;
    if (reply->networkSettings.networks.arg != nullptr) {
        auto array = (pb_array_t *)reply->networkSettings.networks.arg;
        for (size_t i = 0; i < array->length; ++i) {
            fk_app_NetworkInfo *network = &((fk_app_NetworkInfo *)array->buffer)[i];
            network->ssid.funcs.encode = pb_encode_string;
            network->password.funcs.encode = pb_encode_string;
        }
    }
}

int32_t Connection::read(uint8_t *buffer, size_t size) {
    auto bytes = conn_->read(buffer, size);
    return bytes;
}

int32_t Connection::write(uint8_t *buffer, size_t size) {
    auto bytes = conn_->write(buffer, size);
    wrote_ += bytes;
    return bytes;
}

int32_t Connection::write(fk_app_HttpReply *reply) {
    initialize_callbacks(reply);

    size_t size = 0;
    auto fields = fk_app_HttpReply_fields;
    if (!pb_get_encoded_size(&size, fields, reply)) {
        return fault();
    }

    size += pb_varint_size(size);

    logdebug("replying (%d bytes)", size);

    wrote_ += conn_->write("HTTP/1.1 200 OK\n");
    wrote_ += conn_->writef("Content-Length: %zu\n", size);
    wrote_ += conn_->writef("Content-Type: %s\n", "application/octet-stream");
    wrote_ += conn_->write("Connection: close\n");
    wrote_ += conn_->write("\n");

    auto ostream = pb_ostream_from_connection(conn_);
    if (!pb_encode_delimited(&ostream, fields, reply)) {
        return size;
    }

    wrote_ += size;

    req_.finished();

    return size;
}

int32_t Connection::write(const char *s, ...) {
    va_list args;
    va_start(args, s);
    auto r = conn_->vwritef(s, args);
    va_end(args);
    wrote_ += r;
    return r;
}

int32_t Connection::plain(int32_t status, const char *status_description, const char *text) {
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
        .buffer = pool_.copy(errors, sizeof(errors)),
        .fields = fk_app_Error_fields,
    };

    fk_app_HttpReply reply = fk_app_HttpReply_init_default;
    reply.type = fk_app_ReplyType_REPLY_BUSY;
    reply.errors.arg = (void *)pool_.copy(&errors_array, sizeof(errors_array));

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
        .buffer = pool_.copy(errors, sizeof(errors)),
        .fields = fk_app_Error_fields,
    };

    fk_app_HttpReply reply = fk_app_HttpReply_init_default;
    reply.type = fk_app_ReplyType_REPLY_ERROR;
    reply.errors.arg = (void *)pool_.copy(&errors_array, sizeof(errors_array));

    logerror(message);

    return write(&reply);
}

int32_t Connection::close() {
    conn_->stop();
    return 0;
}

}
