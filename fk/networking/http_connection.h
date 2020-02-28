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
    bool begin(const char *method, const char *path, const char *server, uint16_t port);
    void close();

public:
    uint32_t length() const {
        return connection_->length();
    }

    bool active() const {
        return connection_->active();
    }

};

HttpConnection *open_http_connection(const char *method, const char *url, Pool &pool);

}
