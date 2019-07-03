#include <cstring>
#include <malloc.h>
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

    loginfo("checking wifi...");

    if (!wifi_->begin(settings)) {
        loginfo("unable to configure wifi");
        return false;
    }

    while (!wifi_ready(wifi_->status())) {
        fk_delay(100);
    }

    wifi_->serve();

    loginfo("serving");

    return true;
}

void HttpServer::tick() {
    if (pool_.available() > 0) {
        auto connection = wifi_->accept();
        if (connection != nullptr) {
            auto mi = mallinfo();
            loginfo("connection (fd = %ld) (free = %lu) (arena = %zu) (uordblks = %zu)",
                    connection->socket(), fk_free_memory(), mi.arena, mi.uordblks);
            pool_.queue(connection);
        }
    }

    pool_.service();
}

void HttpServer::stop() {
    loginfo("stop");

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
