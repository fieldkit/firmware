#include <os.h>

#include "tasks/tasks.h"
#include "hal/hal.h"
#include "networking/server.h"
#include "state_manager.h"
#include "device_name.h"

namespace fk {

FK_DECLARE_LOGGER("network");

class NetworkTask {
private:
    Network *network_;
    HttpServer *http_server_;
    uint32_t last_checked_configuration_{ 0 };
    uint32_t configuration_modified_{ 0 };

public:
    NetworkTask(Network *network, HttpServer &http_server);
    virtual ~NetworkTask();

public:
    NetworkSettings get_selected_settings(Pool &pool);
    bool did_configuration_change();

};

NetworkTask::NetworkTask(Network *network, HttpServer &http_server) : network_(network), http_server_(&http_server) {
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

    return n.modified != configuration_modified_;
}

NetworkSettings NetworkTask::get_selected_settings(Pool &pool) {
    auto name = fk_device_name_generate(pool);
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
        .name = name,
        .port = 80,
    };
}

void task_handler_network(void *params) {
    while (true) {
        auto &fkc = fk_config();
        auto network = get_network();

        GlobalStateManager gsm;
        MallocPool pool{ "task:network", 256 };
        HttpServer http_server{ network, &fkc };
        NetworkTask task{ network, http_server };

        auto settings = task.get_selected_settings(pool);

        gsm.apply([=](GlobalState *gs) {
            gs->network.state = { };
        });

        // Either create a new AP or try and join an existing one.
        if (settings.valid) {
            if (!http_server.begin(settings, WifiConnectionTimeoutMs, pool)) {
                logerror("error starting server");
                return;
            }
        }
        else {
            if (!http_server.begin(WifiConnectionTimeoutMs, pool)) {
                logerror("error starting server");
                return;
            }
        }

        gsm.apply([&](GlobalState *gs) {
            gs->network.state.ip = get_network()->ip_address();
            gs->network.state.enabled = fk_uptime();
            gs->network.state.connected = 0;
            strncpy(gs->network.state.ssid, http_server.ssid(), sizeof(gs->network.state.ssid));
        });

        // In self AP mode we're waiting for connections now, and hold off doing
        // anything useful until something joins.
        auto started = fk_uptime();
        auto retry = false;
        while (!http_server.ready_to_serve()) {
            if (fk_uptime() - started > fkc.network.uptime) {
                return;
            }
            if (task.did_configuration_change()) {
                retry = true;
                break;
            }
            fk_delay(10);
        }

        if (retry) {
            continue;
        }

        fk_delay(500);

        // Start the network services now that we've got things to talk to.
        if (!http_server.serve()) {
            logerror("error serving");
            continue;
        }

        gsm.apply([=](GlobalState *gs) {
            gs->network.state.enabled = fk_uptime();
            gs->network.state.connected = fk_uptime();
            gs->network.state.ip = get_network()->ip_address();
        });

        loginfo("awaiting connections...");

        while (true) {
            http_server.tick();

            // Some other task has requested that we stop serving. Menu option
            // or a self check for example.
            uint32_t signal = 0;
            if (os_signal_check(&signal) == OSS_SUCCESS) {
                if (signal > 0) {
                    return;
                }
            }

            // Check to see if we've been inactive for too long.
            if (fk_uptime() - http_server.activity() > fkc.network.uptime) {
                return;
            }

            // This will happen when a foreign device disconnects from our WiFi AP.
            if (!http_server.ready_to_serve()) {
                break;
            }

            // Break this loop and go to the beginning to recreate.
            if (task.did_configuration_change()) {
                break;
            }

            fk_delay(10);
        }

        // NetworkTask dtor will fixup our state.
    }
}

}
