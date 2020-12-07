#include <loading.h>
#include "networking/http_server_connection.h"

#include "protobuf.h"
#include "base64.h"
#include "state_ref.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("connection");

HttpServerConnection::HttpServerConnection(Pool *pool, NetworkConnection *conn, uint32_t number, HttpRouter *router) : Connection(pool, conn, number), router_{ router }, req_{ pool_ }, buffer_{ nullptr }, size_{ 0 }, position_{ 0 } {
}

HttpServerConnection::~HttpServerConnection() {
}

int32_t HttpServerConnection::plain(int32_t status, const char *status_description, const char *text) {
    loginfo("[%" PRIu32 "] sending %" PRId32 " '%s'", number_, status, text);

    auto length = strlen(text);

    bytes_tx_ += conn_->writef("HTTP/1.1 %" PRId32 " %s\n", status, status_description);
    bytes_tx_ += conn_->writef("Fk-Connection: #%" PRIu32 "\n", number_);
    bytes_tx_ += conn_->writef("Content-Length: %zu\n", length);
    bytes_tx_ += conn_->write("Content-Type: text/plain\n");
    bytes_tx_ += conn_->write("Connection: close\n");
    bytes_tx_ += conn_->write("\n");
    bytes_tx_ += conn_->write(text);

    req_.finished();

    return 0;
}


int32_t HttpServerConnection::available() const {
    return req_.buffered_body_length();
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
    return plain(500, "internal error", "internal error");
}

int32_t HttpServerConnection::busy(uint32_t delay, const char *message) {
    fk_app_Error errors[] = {
        {
            .message = {
                .funcs = {
                    .encode = pb_encode_string,
                },
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
    reply.errors.funcs.encode = pb_encode_array;
    reply.errors.arg = (void *)pool_->copy(&errors_array, sizeof(errors_array));

    logwarn("[%" PRIu32 "] busy reply '%s'", number_, message);

    return write(503, message, &reply, fk_app_HttpReply_fields);
}

int32_t HttpServerConnection::error(int32_t status, const char *message) {
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
        .buffer = pool_->copy(errors, sizeof(errors)),
        .fields = fk_app_Error_fields,
    };

    fk_app_HttpReply reply = fk_app_HttpReply_init_default;
    reply.type = fk_app_ReplyType_REPLY_ERROR;
    reply.errors.funcs.encode = pb_encode_array;
    reply.errors.arg = (void *)pool_->copy(&errors_array, sizeof(errors_array));

    logwarn("[%" PRIu32 "] error reply '%s'", number_, message);

    return write(status, message, &reply, fk_app_HttpReply_fields);
}

int32_t HttpServerConnection::write(fk_app_HttpReply const *reply) {
    return write(200, "OK", reply, fk_app_HttpReply_fields);
}

int32_t HttpServerConnection::write(int32_t status_code, const char *status_message, void const *record, pb_msgdesc_t const *fields) {
    auto started = fk_uptime();

    size_t size = 0;
    if (!pb_get_encoded_size(&size, fields, record)) {
        return fault();
    }

    size += pb_varint_size(size);

    auto content_size = hex_encoding_ ? size * 2 : size;

    logdebug("[%" PRIu32 "] replying (%zd bytes)", number_, content_size);

    bytes_tx_ += conn_->writef("HTTP/1.1 %" PRId32 " %s\n", status_code, status_message);
    bytes_tx_ += conn_->writef("Fk-Connection: #%" PRIu32 "\n", number_);
    bytes_tx_ += conn_->writef("Content-Length: %zu\n", content_size);
    bytes_tx_ += conn_->writef("Content-Type: %s\n", "application/octet-stream");
    bytes_tx_ += conn_->write("Connection: close\n");
    bytes_tx_ += conn_->write("\n");

    logdebug("[%" PRIu32 "] headers done (%" PRIu32 "ms)", number_, fk_uptime() - started);

    StackBufferedWriter<StackBufferSize> buffered{ this };
    Base64Writer b64_writer{ &buffered };
    Writer *writer = &buffered;

    if (hex_encoding_) {
        writer = &b64_writer;
    }

    auto ostream = pb_ostream_from_writable(writer);
    if (!pb_encode_delimited(&ostream, fields, record)) {
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

    if (router_ != nullptr) {
        if (req_.have_headers()) {
            if (!routed_) {
                auto path = req_.url_parser().path();
                if (path == nullptr) {
                    auto gs = get_global_state_ro();
                    plain(200, "ok", pool_->sprintf("Hello! I am a FieldKit station. My name is %s.\nFirmware: %s (#%" PRIu32 ")\n",
                                                    gs.get()->general.name,
                                                    fkb_header.firmware.name, fkb_header.firmware.number));
                    return true;
                }

                loginfo("[%" PRIu32 "] routing '%s' path = '%s' (%" PRIu32 " bytes) ('%s')",
                        number_, req_.url(), path, req_.length(), req_.user_agent());

                auto handler = router_->route(path);
                if (handler == nullptr) {
                    plain(404, "not found", "404: not found, no handler");
                }
                else {
                    if (!handler->handle(this, *pool_)) {
                        plain(500, "internal error", "500: internal error");
                    }
                }
                routed_ = true;
                activity_ = fk_uptime();
            }
        }
    }

    return true;
}

}
