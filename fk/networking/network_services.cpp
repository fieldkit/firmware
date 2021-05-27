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
#include "tasks/tasks.h"

namespace fk {

FK_DECLARE_LOGGER("ns");

static DefaultRoutes default_routes;

static bool network_began(NetworkStatus status) {
    return status == NetworkStatus::Connected || status == NetworkStatus::Listening;
}

NetworkServices::NetworkServices(Network *network) : network_(network) {
    started_ = fk_uptime();
}

NetworkServices::~NetworkServices() {
    loginfo("dtor");
    stop();
}

bool NetworkServices::enabled() const {
    return network_->enabled();
}

const char *NetworkServices::ssid() const {
    return active_settings_.ssid;
}

bool NetworkServices::active_connections() const {
    return connection_pool_.active_connections();
}

uint32_t NetworkServices::activity() const {
    return connection_pool_.activity();
}

uint32_t NetworkServices::bytes_rx() const {
    return connection_pool_.bytes_rx();
};

uint32_t NetworkServices::bytes_tx() const {
    return connection_pool_.bytes_tx();
};


bool NetworkServices::waiting_to_serve() {
    if (network_->status() == NetworkStatus::Connected) {
        loginfo("connected");
        return false;
    }

    if (did_configuration_change()) {
        loginfo("configuration changed");
        return false;
    }

    return true;
}

bool NetworkServices::can_serve() const {
    return network_->status() == NetworkStatus::Connected;
}

bool NetworkServices::serving() {
    // Break this loop and go to the beginning to recreate.
    if (did_configuration_change()) {
        loginfo("stopping: configuration");
        return false;
    }

    // This will happen when a foreign device disconnects from
    // our WiFi AP and in that case we keep the WiFi on until
    // we're properly considered inactive.
    if (network_->status() != NetworkStatus::Connected) {
        if (duration_.always_on()) {
            loginfo("stopping: disconnected (always-on)");
        }
        else {
            loginfo("stopping: disconnected");
        }
        return false;
    }

    return true;
}

bool NetworkServices::should_stop() {
    // Check to see if we've been inactive for too long.
    if (!duration_.on(std::max(started_, activity()))) {
        loginfo("inactive");
        return true;
    }

    // Some other task has requested that we stop serving. Menu option
    // or a self check for example.
    if (fk_task_stop_requested(&signal_checked_)) {
        loginfo("stop requested");
        return true;
    }

    return false;
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
        network_->service(nullptr);

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
    auto lock = wifi_mutex.acquire(UINT32_MAX);
    FK_ASSERT(lock);

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

void NetworkServices::tick() {
    auto lock = wifi_mutex.acquire(UINT32_MAX);
    FK_ASSERT(lock);

    if (connection_pool_.active_connections()) {
        network_->service(nullptr);
    }
    else {
        network_->service(&tick_pool_);

        if (tick_pool_.used() > 0) {
            loginfo("network-tick: %zu/%zu", tick_pool_.used(), tick_pool_.size());
            tick_pool_.clear();
        }
    }

    if (network_->status() == NetworkStatus::Connected) {
        if (connection_pool_.available() > 0) {
            auto http_connection = http_listener_->get()->accept();
            if (http_connection != nullptr) {
                connection_pool_.queue_http(http_connection);
            }

            auto debug_connection = debug_listener_->get()->accept();
            if (debug_connection != nullptr) {
                connection_pool_.queue_debug(debug_connection);
            }
        }

        connection_pool_.service();
    }
}

void NetworkServices::stop() {
    auto lock = wifi_mutex.acquire(UINT32_MAX);
    FK_ASSERT(lock);

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

bool NetworkServices::begin(uint32_t to, Pool &pool) {
    auto settings = get_selected_settings(pool);
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

    duration_ = gs.get()->scheduler.network.duration;

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
