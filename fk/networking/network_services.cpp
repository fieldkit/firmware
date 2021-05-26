#include <cstring>
#include <malloc.h>

#include "common.h"
#include "platform.h"
#include "networking/network_services.h"
#include "networking/default_routes.h"
#include "hal/hal.h"
#include "device_name.h"
#include "state_ref.h"
#include "state_manager.h"

namespace fk {

FK_DECLARE_LOGGER("ns");

static DefaultRoutes default_routes;

static bool network_began(NetworkStatus status) {
    return status == NetworkStatus::Connected || status == NetworkStatus::Listening;
}

NetworkServices::NetworkServices(Network *network) : network_(network) {
}

NetworkServices::~NetworkServices() {
    loginfo("dtor");
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
    return active_settings_.ssid;
}

bool NetworkServices::try_begin(NetworkSettings settings, uint32_t to, Pool &pool) {
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
    loginfo("stopping...");

    network_->stop();

    GlobalStateManager gsm;
    gsm.apply([=](GlobalState *gs) {
        gs->network.state = { };
    });

    loginfo("stopped");
}

static collection<NetworkSettings> copy_settings(Pool &pool) {
    collection<NetworkSettings> settings{ pool };

    auto gs = get_global_state_ro();
    for (auto &wifi_network : gs.get()->network.config.wifi_networks) {
        if (wifi_network.ssid[0] != 0) {
            settings.add({
                .valid = wifi_network.ssid[0] != 0,
                .create = false,
                .ssid = wifi_network.ssid,
                .password = wifi_network.password,
                .port = 80,
            });
        }
    }

    auto name = pool.strdup(gs.get()->general.name);
    settings.add({
        .valid = true,
        .create = true,
        .ssid = name,
        .password = nullptr,
        .port = 80,
    });

    return settings;
}

bool NetworkServices::begin(NetworkSettings settings, uint32_t to, Pool &pool) {
    if (settings.valid) {
        if (try_begin(settings, to, pool)) {
            active_settings_ = settings;
            return true;
        }
    }

    auto network_settings = copy_settings(pool);
    for (NetworkSettings &s : network_settings) {
        if (try_begin(s, to, pool)) {
            active_settings_ = s;
            return true;
        }
    }

    return false;
}

bool NetworkServices::did_configuration_change() {
    if (fk_uptime() - last_checked_configuration_ < ConfigurationCheckIntervalMs) {
        return false;
    }

    auto gs = get_global_state_ro();
    auto modified = gs.get()->network.config.modified;
    auto changed = false;

    if (modified != configuration_modified_) {
        loginfo("configuration modified");

        duration_ = gs.get()->scheduler.network.duration;
        changed = true;
    }

    if (false) {
        if (active_settings_.create) {
            if (strncmp(gs.get()->general.name, active_settings_.ssid, sizeof(gs.get()->general.name)) != 0) {
                loginfo("name changed '%s' vs '%s'", gs.get()->general.name, active_settings_.ssid);
                changed = true;
            }
        }
    }

    last_checked_configuration_ = fk_uptime();

    return changed;
}

NetworkSettings NetworkServices::get_selected_settings(Pool &pool) {
    auto gs = get_global_state_ro();
    auto &n = gs.get()->network.config.selected;
    if (!n.valid) {
        return {
            .valid = false,
            .create = false,
            .ssid = nullptr,
            .password = nullptr,
            .port = 80,
        };
    }

    auto modified = gs.get()->network.config.modified;

    configuration_modified_ = modified;

    return {
        .valid = true,
        .create = n.create,
        .ssid = n.ssid,
        .password = n.password,
        .port = 80,
    };
}

}
