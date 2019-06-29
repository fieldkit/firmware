#include <cstring>

#include "common.h"
#include "platform.h"
#include "httpd/server.h"
#include "hal/hal.h"

namespace fk {

static bool wifi_ready(WifiStatus status) {
    return status == WifiStatus::Connected || status == WifiStatus::Listening;
}

HttpServer::HttpServer(Wifi *wifi) : wifi_(wifi), ssid_(nullptr), password_(nullptr) {
}

HttpServer::HttpServer(Wifi *wifi, const char *ssid, const char *password) : wifi_(wifi), ssid_(ssid), password_(password) {
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

bool HttpServer::begin() {
    auto settings = get_settings();

    if (!wifi_->begin(settings)) {
        fkinfo("unable to configure wifi");
        return false;
    }

    fkinfo("waiting on wifi...");

    while (!wifi_ready(wifi_->status())) {
        fk_delay(100);
    }


    fkinfo("serving");

    wifi_->serve();

    return true;
}

void HttpServer::tick() {
    auto connection = wifi_->accept();
    if (connection == nullptr) {
        return;
    }

    fkinfo("connection!");

    uint8_t buffer[1024] = { 0 };
    size_t position = 0;
    HttpRequest req;

    // TODO: 414 Request-URI Too Long
    // TODO: 431 Request Header Fields Too Large.
    // TODO: 413 Payload Too Large
    // TODO: 500 Service Unavailable
    // TODO: 503 Service Unavailable

    while (connection->status() == WifiConnectionStatus::Connected) {
        if (connection->available()) {
            auto available = sizeof(buffer) - position;
            auto nread = connection->read(buffer, available);
            if (nread < 0) {
                fkinfo("EOS read");
                continue;
            }
            if (nread == 0) {
                fkinfo("empty read");
                continue;
            }

            if (req.parse((const char *)(buffer + position), nread) != 0) {
            }

            if (req.have_headers()) {
            }

            if (req.consumed()) {
                connection->write("HTTP/1.1 204 OK\n");
                connection->write("Content-Length: 0\n");
                connection->write("Content-Type: text/html\n");
                connection->write("Connection: close\n");
                connection->write("\n");
                break;
            }

            position += nread;
        }
    }

    connection->stop();
}

void HttpServer::stop() {
    fkinfo("http stopping");

    wifi_->stop();
}

}
