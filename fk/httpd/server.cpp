#include <cstring>
#include <os.h>

#include "common.h"
#include "platform.h"
#include "httpd/server.h"
#include "hal/hal.h"

namespace fk {

#define loginfo(f, ...)  loginfof("httpd", f, ##__VA_ARGS__)

#define logerror(f, ...) logerrorf("httpd", f, ##__VA_ARGS__)

static bool wifi_ready(WifiStatus status) {
    return status == WifiStatus::Connected || status == WifiStatus::Listening;
}

HttpServer::HttpServer(Wifi *wifi) : wifi_(wifi), ssid_(nullptr), password_(nullptr) {
}

HttpServer::HttpServer(Wifi *wifi, const char *ssid, const char *password) : wifi_(wifi), ssid_(ssid), password_(password) {
}

bool HttpServer::begin() {
    auto settings = get_settings();

    if (!wifi_->begin(settings)) {
        loginfo("unable to configure wifi");
        return false;
    }

    loginfo("waiting on wifi...");

    while (!wifi_ready(wifi_->status())) {
        fk_delay(100);
    }

    loginfo("serving");

    wifi_->serve();

    return true;
}

class ConnectionPool {
private:
    typedef struct Connection {
        WifiConnection *c{ nullptr };
        HttpRequest req;
        uint8_t data[1024];
        size_t position;
    } Connection;

    constexpr static size_t MaximumConnections = 4;
    Connection pool_[MaximumConnections];

public:
    size_t available() {
        size_t used = 0;
        for (auto i = (size_t)0; i < MaximumConnections; ++i) {
            if (pool_[i].c != nullptr) {
                used++;
            }
        }
        return MaximumConnections - used;
    }

    void service() {
        for (auto i = (size_t)0; i < MaximumConnections; ++i) {
            if (pool_[i].c != nullptr) {
                if (!service(&pool_[i])) {
                    // loginfo("%d removing", i);
                    pool_[i].c->stop();
                    delete pool_[i].c;
                    pool_[i] = { };
                }
            }
        }
    }

    void queue(WifiConnection *c) {
        for (auto i = (size_t)0; i < MaximumConnections; ++i) {
            if (pool_[i].c == nullptr) {
                // loginfo("%d queued", i);
                pool_[i].c = c;
                pool_[i].position = 0;
                pool_[i].req.begin();
                return;
            }
        }
        FK_ASSERT(false);
    }

private:
    bool service(Connection *c) {
        // TODO: 414 Request-URI Too Long
        // TODO: 431 Request Header Fields Too Large.
        // TODO: 413 Payload Too Large
        // TODO: 500 Service Unavailable
        // TODO: 503 Service Unavailable
        if (c->c->status() != WifiConnectionStatus::Connected) {
            return false;
        }

        if (c->c->available()) {
            auto available = (sizeof(c->data) - 1) - c->position;
            auto nread = c->c->read(c->data, available);
            if (nread < 0) {
                loginfo("EOS read");
                return false;
            }

            if (nread == 0) {
                loginfo("empty read");
                return false;
            }

            auto ptr = (char *)(c->data + c->position);
            ptr[nread] = 0;
            if (c->req.parse(ptr, nread) != 0) {
                return false;
            }

            c->position += nread;
        }

        if (c->req.have_headers()) {
            loginfo("routing '%s' (%lu bytes)", c->req.url(), c->req.length());
        }

        if (c->req.consumed()) {
            c->c->write("HTTP/1.1 204 OK\n");
            c->c->write("Content-Length: 0\n");
            c->c->write("Content-Type: text/html\n");
            c->c->write("Connection: close\n");
            c->c->write("\n");
            return false;
        }

        return true;
    }

};

ConnectionPool pool;

void HttpServer::tick() {
    if (pool.available() > 0) {
        auto connection = wifi_->accept();
        if (connection != nullptr) {
            loginfo("connection (0x%p) %ld", connection, connection->socket());
            pool.queue(connection);
        }
    }

    pool.service();
}

void HttpServer::stop() {
    loginfo("stopping");

    wifi_->stop();
}

WifiSettings HttpServer::get_settings() {
    if (ssid_ == nullptr) {
        return {
            .create = true,
            .ssid = "FkDevice",
            .password = nullptr,
            .name = "FkDevice",
            .port = 80,
        };
    }
    return {
        .create = false,
        .ssid = ssid_,
        .password = password_,
        .name = "FK-DEVICE",
        .port = 80,
    };
}

}
