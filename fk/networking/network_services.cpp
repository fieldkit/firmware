#include <cstring>
#include <malloc.h>

#include "common.h"
#include "platform.h"
#include "networking/network_services.h"
#include "networking/default_routes.h"
#include "hal/hal.h"
#include "device_name.h"

namespace fk {

FK_DECLARE_LOGGER("httpd");

static DefaultRoutes default_routes;

static bool network_began(NetworkStatus status) {
    return status == NetworkStatus::Connected || status == NetworkStatus::Listening;
}

NetworkServices::NetworkServices(Network *network) : network_(network) {
}

NetworkServices::~NetworkServices() {
    loginfo("stopping");
    stop();
}

bool NetworkServices::enabled() const {
    return network_->enabled();
}

uint32_t NetworkServices::activity() const {
    return pool_.activity();
}

bool NetworkServices::active_connections() const {
    return pool_.active_connections();
}

bool NetworkServices::ready_to_serve() const {
    return network_->status() == NetworkStatus::Connected;
}

const char *NetworkServices::ssid() const {
    return settings_.ssid;
}

bool NetworkServices::begin(NetworkSettings settings, uint32_t to, Pool &pool) {
    if (settings.create) {
        loginfo("creating '%s'", settings.ssid);
    }
    else {
        loginfo("trying '%s'", settings.ssid);
    }

    if (!network_->begin(settings, &pool)) {
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

bool NetworkServices::serve() {
    default_routes.add_routes(router_);

    http_listener_ = network_->listen(80);
    if (http_listener_ == nullptr) {
        return false;
    }

    debug_listener_ = network_->listen(23);
    if (debug_listener_ == nullptr) {
        return false;
    }

    if (!network_->serve()) {
        return false;
    }

    loginfo("serving");

    return true;
}

void NetworkServices::tick(Pool *pool) {
    if (pool_.available() > 0) {
        auto http_connection = http_listener_->get()->accept();
        if (http_connection != nullptr) {
            pool_.queue_http(http_connection);
        }

        auto debug_connection = debug_listener_->get()->accept();
        if (debug_connection != nullptr) {
            pool_.queue_debug(debug_connection);
        }
    }

    network_->service(pool);

    pool_.service();
}

void NetworkServices::stop() {
    network_->stop();
}

}
