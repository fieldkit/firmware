#include "networking/network_task.h"

namespace fk {

FK_DECLARE_LOGGER("network");

NetworkTask::NetworkTask(Network *network, NetworkServices &network_services) : network_(network), network_services_(&network_services) {
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

    return std::move(settings);
}

bool NetworkTask::begin(NetworkSettings settings, uint32_t to, Pool &pool) {
    if (settings.valid) {
        if (network_services_->begin(settings, to, pool)) {
            active_settings_ = settings;
            return true;
        }
    }

    auto network_settings = copy_settings(pool);
    for (NetworkSettings &s : network_settings) {
        if (network_services_->begin(s, to, pool)) {
            active_settings_ = s;
            return true;
        }
    }

    return false;
}

NetworkTask::~NetworkTask() {
    loginfo("network stopping...");

    network_->stop();

    loginfo("network stopped");

    GlobalStateManager gsm;
    gsm.apply([=](GlobalState *gs) {
        gs->network.state = { };
    });

    loginfo("done");
}

bool NetworkTask::did_configuration_change() {
    if (fk_uptime() - last_checked_configuration_ < ConfigurationCheckIntervalMs) {
        return false;
    }

    auto gs = get_global_state_ro();
    auto modified = gs.get()->network.config.modified;
    auto changed = false;

    last_checked_configuration_ = fk_uptime();

    if (modified != configuration_modified_) {
        loginfo("modified (version)");
        changed = true;
    }

    if (active_settings_.create) {
        if (strncmp(gs.get()->general.name, active_settings_.ssid, sizeof(gs.get()->general.name)) != 0) {
            loginfo("name changed '%s' vs '%s'", gs.get()->general.name, active_settings_.ssid);
            changed = true;
        }
    }

    return changed;
}

NetworkSettings NetworkTask::get_selected_settings(Pool &pool) {
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
