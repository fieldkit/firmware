#include <cstring>
#include <malloc.h>

#include "common.h"
#include "platform.h"
#include "networking/server.h"
#include "networking/default_routes.h"
#include "hal/hal.h"
#include "device_name.h"

namespace fk {

FK_DECLARE_LOGGER("httpd");

static DefaultRoutes default_routes;

static bool network_began(NetworkStatus status) {
    return status == NetworkStatus::Connected || status == NetworkStatus::Listening;
}

HttpServer::HttpServer(Network *network) : network_(network) {
}

HttpServer::~HttpServer() {
    loginfo("stopping");
    stop();
}

bool HttpServer::enabled() const {
    return network_->enabled();
}

uint32_t HttpServer::activity() const {
    return pool_.activity();
}

bool HttpServer::active_connections() const {
    return pool_.active_connections();
}

bool HttpServer::ready_to_serve() const {
    return network_->status() == NetworkStatus::Connected;
}

const char *HttpServer::ssid() const {
    return settings_.ssid;
}

bool HttpServer::begin(NetworkSettings settings, uint32_t to, Pool &pool) {
    if (settings.create) {
        loginfo("creating '%s'", settings.ssid);
    }
    else {
        loginfo("trying '%s'", settings.ssid);
    }

    if (!network_->begin(settings)) {
        loginfo("unable to configure network");
        return false;
    }

    auto started = fk_uptime();
    do {
        if (network_began(network_->status())) {
            settings_ = settings;
            return true;
        }

        fk_delay(100);
    }
    while (fk_uptime() - started < to);

    logerror("networking took too long");
    return false;
}

bool HttpServer::serve() {
    default_routes.add_routes(router_);

    if (!network_->serve()) {
        return false;
    }

    http_listener_ = network_->listen(80);
    if (http_listener_ == nullptr) {
        return false;
    }

    loginfo("serving");

    return true;
}

void HttpServer::tick() {
    if (pool_.available() > 0) {
        auto connection = http_listener_->get()->accept();
        if (connection != nullptr) {
            pool_.queue(connection);
        }
    }

    network_->service();

    pool_.service(router_);
}

void HttpServer::stop() {
    network_->stop();
}

}
