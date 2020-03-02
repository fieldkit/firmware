#pragma once

#include "networking/http_server_connection.h"

namespace fk {

class HttpConnection : public Reader, public Writer {
private:
    PoolPointer<NetworkConnection> *nc_;
    HttpServerConnection *connection_;

public:
    HttpConnection(PoolPointer<NetworkConnection> *nc);
    virtual ~HttpConnection();

public:
    int32_t write(uint8_t const *buffer, size_t size) override;
    int32_t read(uint8_t *buffer, size_t size) override;

public:
    bool begin(const char *scheme, const char *method, const char *path, const char *server, uint16_t port, const char *extra_headers, bool expecting_headers);
    bool read_headers();
    bool read_response();
    void close();

public:
    uint32_t length() const {
        return connection_->length();
    }

    int32_t status_code() const {
        return connection_->status_code();
    }

    bool active() const {
        return connection_->active();
    }

};

HttpConnection *open_http_connection(const char *method, const char *url, const char *extra_headers, bool expecting_headers, Pool &pool);

}
