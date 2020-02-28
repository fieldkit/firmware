#include "networking/http_connection.h"
#include "networking/http_server_connection.h"

#include "protobuf.h"
#include "base64.h"

namespace fk {

FK_DECLARE_LOGGER("connection");

HttpConnection::HttpConnection(PoolPointer<NetworkConnection> *nc) : nc_(nc) {
    connection_ = new (nc_->pool()) HttpServerConnection(nc_->pool(), nc_->get(), 0, nullptr);
}

HttpConnection::~HttpConnection() {
    close();
}

int32_t HttpConnection::write(uint8_t const *buffer, size_t size) {
    if (!connection_->active()) {
        return -1;
    }

    if (!connection_->service()) {
        return -1;
    }

    return connection_->write(buffer, size);
}

int32_t HttpConnection::read(uint8_t *buffer, size_t size) {
    if (!connection_->active()) {
        return -1;
    }

    if (!connection_->service()) {
        return -1;
    }

    return connection_->read(buffer, size);
}

bool HttpConnection::begin(const char *method, const char *path, const char *server, uint16_t port) {
    connection_->printf("%s /%s HTTP/1.1\r\n"
                        "Host: %s:%d\r\n"
                        "Connection: close\r\n"
                        "Accept: */*\r\n\r\n", method, path, server, port);

    while (connection_->active()) {
        if (!connection_->service()) {
            break;
        }

        if (connection_->have_headers()) {
            return true;
        }
    }

    close();

    return false;
}

void HttpConnection::close() {
    if (nc_ != nullptr) {
        connection_->close();
        nc_->get()->stop();
        delete nc_;
        nc_ = nullptr;
    }
}

HttpConnection *open_http_connection(const char *method, const char *url, Pool &pool) {
    UrlParser url_parser{ pool.strdup(url) };

    loginfo("connecting to: %s:%d", url_parser.server(), url_parser.port());

    auto nc = get_network()->open_connection(url_parser.server(), url_parser.port());
    if (nc == nullptr) {
        logerror("connection error");
        return nullptr;
    }

    auto http = new (pool) HttpConnection(nc);

    loginfo("beginning %s %s", method, url_parser.path());

    if (!http->begin(method, url_parser.path(), url_parser.server(), url_parser.port())) {
        http->close();
        return nullptr;
    }

    return http;
}

}
