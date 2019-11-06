#include "networking/network_task.h"
#include "device_name.h"

namespace fk {

FK_DECLARE_LOGGER("network");

NetworkTask::NetworkTask(Network *network, HttpServer &http_server) : network_(network), http_server_(&http_server) {
}

bool NetworkTask::begin(NetworkSettings settings, uint32_t to, Pool &pool) {
    if (settings.valid) {
        if (http_server_->begin(settings, to, pool)) {
            active_settings_ = settings;
            return true;
        }
    }

    auto gs = get_global_state_ro();
    auto name = gs.get()->general.name;
    for (auto &wifi_network : gs.get()->network.config.wifi_networks) {
        auto s = NetworkSettings{
            .valid = wifi_network.ssid[0] != 0,
            .create = false,
            .ssid = wifi_network.ssid,
            .password = wifi_network.password,
            .port = 80,
        };

        if (s.valid) {
            if (http_server_->begin(s, to, pool)) {
                active_settings_ = s;
                return true;
            }
        }
    }

    auto s = NetworkSettings{
        .valid = true,
        .create = true,
        .ssid = name,
        .password = nullptr,
        .port = 80,
    };

    if (http_server_->begin(s, to, pool)) {
        active_settings_ = s;
        return true;
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
    if (fk_uptime() - last_checked_configuration_ < 500) {
        return false;
    }

    auto gs = get_global_state_ro();
    auto n = gs.get()->network.config.selected;

    last_checked_configuration_ = fk_uptime();

    if (n.modified != configuration_modified_) {
        loginfo("modified");
        return true;
    }

    if (!active_settings_.create) {
        return false;
    }

    if (strncmp(gs.get()->general.name, active_settings_.ssid, sizeof(gs.get()->general.name)) != 0) {
        loginfo("name changed '%s' vs '%s'", gs.get()->general.name, active_settings_.ssid);
        return true;
    }

    return false;
}

NetworkSettings NetworkTask::get_selected_settings(Pool &pool) {
    auto gs = get_global_state_ro();
    auto &n = gs.get()->network.config.selected;
    if (!n.valid) {
        return {
            .valid = false,
        };
    }

    configuration_modified_ = n.modified;

    return {
        .valid = true,
        .create = n.create,
        .ssid = n.ssid,
        .password = n.password,
        .port = 80,
    };
}

}
