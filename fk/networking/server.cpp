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

HttpServer::HttpServer(Network *network, configuration_t const *fkc) : network_(network), fkc_(fkc) {
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

bool HttpServer::begin(uint32_t to, Pool &pool) {
    loginfo("starting network...");

    auto name = fk_device_name_generate(pool);
    if (!try_configurations(name, to)) {
        return false;
    }

    default_routes.add_routes(router_);

    return true;
}

bool HttpServer::serve() {
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

bool HttpServer::try_configurations(const char *name, uint32_t to) {
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
            if (network_began(network_->status())) {
                ready = true;
                break;
            }

            fk_delay(100);

            if (fk_uptime() - started > to) {
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
