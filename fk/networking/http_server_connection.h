#pragma once

#include "networking/connection.h"
#include "networking/routing.h"
#include "networking/req.h"

namespace fk {

enum class HttpStatus : int32_t {
    Ok = 200,
    BadRequest = 400,
    NotFound = 404,
    ServerError = 500,
    Busy = 503,
};

class HttpServerConnection : public Connection {
private:
    HttpRouter *router_;
    HttpRequest req_;
    uint8_t *buffer_;
    size_t size_;
    size_t position_;
    bool routed_{ false };
    bool hex_encoding_{ false };

public:
    HttpServerConnection(Pool *pool, NetworkConnection *conn, uint32_t number, HttpRouter *router);
    virtual ~HttpServerConnection();

public:
    void hex_encoding(bool hex_encoding) {
        hex_encoding_ = hex_encoding;
    }

    bool service() override;

    int32_t read(uint8_t *buffer, size_t size) override;

    int32_t write(HttpStatus status, const char *status_message, void const *record, pb_msgdesc_t const *fields, Pool &pool);
    int32_t write(HttpStatus status, const char *status_message, uint8_t const *data, size_t size, Pool &pool);

    int32_t write(fk_app_HttpReply const *reply, Pool &pool);

    int32_t plain(HttpStatus status, const char *status_description, const char *text, Pool &pool);
    int32_t error(HttpStatus status, const char *message, Pool &pool);

    int32_t busy(uint32_t retry, const char *message, Pool &pool);

    int32_t fault(Pool &pool);

    using Connection::write;

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

    bool is_get_method() const {
        return req_.is_get_method();
    }

    bool is_head_method() const {
        return req_.is_head_method();
    }

    bool have_headers() const {
        return req_.have_headers();
    }

    int32_t status_code() const {
        return req_.status_code();
    }

    int32_t available() const;

};

}
