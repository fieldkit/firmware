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

static bool network_ready(NetworkStatus status) {
    return status == NetworkStatus::Connected || status == NetworkStatus::Listening;
}

static bool network_ready_to_serve(NetworkStatus status) {
    return status == NetworkStatus::Connected;
}

HttpServer::HttpServer(Network *network, configuration_t const *fkc) : network_(network), fkc_(fkc) {
}

HttpServer::~HttpServer() {
    stop();
}

bool HttpServer::begin(NetworkRunningCallback *callback) {
    StaticPool<128> pool{ "http:begin "};

    loginfo("checking network...");

    auto name = fk_device_name_generate(pool);
    if (!try_configurations(name)) {
        return false;
    }

    while (!network_ready_to_serve(network_->status())) {
        if (!callback->running()) {
            return false;
        }

        fk_delay(10);
    }

    fk_delay(1000);

    loginfo("ready to serve...");

    default_routes.add_routes(router_);

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
            pool_.queue(connection);
        }
    }

    pool_.service(router_);
}

void HttpServer::stop() {
    network_->stop();
}

bool HttpServer::try_configurations(const char *name) {
    for (auto &config : fkc_->network.networks) {
        auto settings = get_settings(config, name);

        loginfo("trying '%s'", settings.ssid);

        if (!network_->begin(settings)) {
            loginfo("unable to configure network");
            continue;
        }

        auto started = fk_uptime();
        auto ready = false;
        while (!ready) {
            if (network_ready(network_->status())) {
                ready = true;
                break;
            }

            fk_delay(100);

            if (fk_uptime() - started > WifiConnectionTimeoutMs) {
                logerror("networking took too long");
                break;
            }
        }

        if (ready) {
            return true;
        }
    }

    return false;
}

NetworkSettings HttpServer::get_settings(configuration_t::wifi_network_t const &network, const char *name) {
    if (network.ssid == nullptr) {
        return {
            .create = true,
            .ssid = name,
            .password = nullptr,
            .name = name,
            .port = 80,
        };
    }
    return {
        .create = false,
        .ssid = network.ssid,
        .password = network.password,
        .name = name,
        .port = 80,
    };
}

}
