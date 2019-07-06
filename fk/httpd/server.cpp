#include <cstring>
#include <malloc.h>

#include "common.h"
#include "platform.h"
#include "httpd/server.h"
#include "hal/hal.h"

namespace fk {

#define loginfo(f, ...)  loginfof("httpd", f, ##__VA_ARGS__)

#define logerror(f, ...) logerrorf("httpd", f, ##__VA_ARGS__)

static bool network_ready(NetworkStatus status) {
    return status == NetworkStatus::Connected || status == NetworkStatus::Listening;
}

HttpServer::HttpServer(Network *network) : network_(network), ssid_(nullptr), password_(nullptr) {
}

HttpServer::HttpServer(Network *network, const char *ssid, const char *password) : network_(network), ssid_(ssid), password_(password) {
}

bool HttpServer::begin() {
    auto settings = get_settings();

    loginfo("checking network...");

    if (!network_->begin(settings)) {
        loginfo("unable to configure network");
        return false;
    }

    auto started = fk_uptime();
    while (!network_ready(network_->status())) {
        fk_delay(100);

        if (fk_uptime() - started > WifiConnectionTimeoutMs) {
            logerror("networking took too long");
            return false;
        }
    }

    if (!network_->serve()) {
        return false;
    }

    loginfo("serving");

    return true;
}

void HttpServer::tick() {
    if (pool_.available() > 0) {
        auto connection = network_->accept();
        if (connection != nullptr) {
            auto mi = mallinfo();
            loginfo("connection (fd = %" PRId32 ") (free = %" PRIu32 ") (arena = %d) (uordblks = %d)",
                    connection->socket(), fk_free_memory(), mi.arena, mi.uordblks);
            pool_.queue(connection);
        }
    }

    pool_.service();
}

void HttpServer::stop() {
    loginfo("stop");

    network_->stop();
}

NetworkSettings HttpServer::get_settings() {
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
