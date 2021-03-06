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
    if (connection_->available() > 0) {
        return connection_->read(buffer, size);
    }

    if (!connection_->active()) {
        return -1;
    }

    if (!connection_->service()) {
        return -1;
    }

    return connection_->read(buffer, size);
}

bool HttpConnection::begin(const char *scheme, const char *method, const char *path, const char *server, uint16_t port, const char *extra_headers, bool expecting_headers) {
    connection_->printf("%s /%s HTTP/1.1\r\n"
                        "Host: %s:%d\r\n"
                        "Connection: close\r\n"
                        "Accept: */*\r\n%s\r\n", method, path, server, port, extra_headers);

    // This was useful when the server would response immediately w/o
    // waiting for the body of the request. This isn't the case when
    // uploading data, for example but would be the case when
    // downloading firmware.
    if (expecting_headers) {
        return read_headers();
    }

    return true;
}

bool HttpConnection::read_headers() {
    while (connection_->active()) {
        if (connection_->have_headers()) {
            return true;
        }

        if (!connection_->service()) {
            break;
        }
    }

    return false;
}

bool HttpConnection::read_response() {
    auto success = false;

    if (read_headers()) {
        success = true;
    } else {
        logwarn("no headers");
    }

    return success;
}

void HttpConnection::close() {
    if (nc_ != nullptr) {
        connection_->close();
        nc_->get()->stop();
        delete nc_;
        nc_ = nullptr;
    }
}

HttpConnection *open_http_connection(const char *method, const char *url, const char *extra_headers, bool expecting_headers, Pool &pool) {
    UrlParser url_parser{ pool.strdup(url) };

    loginfo("connecting scheme=%s server=%s %d", url_parser.scheme(), url_parser.server(), url_parser.port());

    auto nc = get_network()->open_connection(url_parser.scheme(), url_parser.server(), url_parser.port());
    if (nc == nullptr) {
        logerror("connection error");
        return nullptr;
    }

    auto http = new (pool) HttpConnection(nc);

    loginfo("beginning %s %s", method, url_parser.path());

    if (!http->begin(url_parser.scheme(), method, url_parser.path(), url_parser.server(), url_parser.port(), extra_headers, expecting_headers)) {
        http->close();
        return nullptr;
    }

    return http;
}

}
