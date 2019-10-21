#include <os.h>

#include "tasks/tasks.h"
#include "hal/hal.h"
#include "networking/network_task.h"

namespace fk {

FK_DECLARE_LOGGER("network");

void task_handler_network(void *params) {
    while (true) {
        auto &fkc = fk_config();
        auto network = get_network();

        GlobalStateManager gsm;
        HttpServer http_server{ network };
        NetworkTask task{ network, http_server };

        // NOTE Can we just stack allocate this?
        auto pool = create_pool_inside("network", DefaultWorkerPoolSize);
        auto settings = task.get_selected_settings(*pool);

        gsm.apply([=](GlobalState *gs) {
            gs->network.state = { };
        });

        loginfo("starting network...");

        if (!task.begin(settings, WifiConnectionTimeoutMs, *pool)) {
            logerror("error starting server");
            return;
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

            // Some other task has requested that we stop serving. Menu option
            // or a self check for example.
            uint32_t signal = 0;
            if (os_signal_check(&signal) == OSS_SUCCESS) {
                if (signal > 0) {
                    return;
                }
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

        auto statistics_update = fk_uptime() + 1000;

        while (true) {
            http_server.tick();

            // Some other task has requested that we stop serving. Menu option
            // or a self check for example.
            uint32_t signal = 0;
            if (os_signal_check(&signal) == OSS_SUCCESS) {
                if (signal > 0) {
                    loginfo("killed");
                    return;
                }
            }

            // Check to see if we've been inactive for too long.
            if (fk_uptime() - http_server.activity() > fkc.network.uptime) {
                loginfo("inactive");
                return;
            }

            // This will happen when a foreign device disconnects from our WiFi AP.
            if (!http_server.ready_to_serve()) {
                loginfo("disconnected");
                break;
            }

            // Break this loop and go to the beginning to recreate.
            if (task.did_configuration_change()) {
                loginfo("configuration change");
                break;
            }

            if (fk_uptime() > statistics_update) {
                gsm.apply([&](GlobalState *gs) {
                    gs->network.state.bytes_rx = http_server.bytes_rx();
                    gs->network.state.bytes_tx = http_server.bytes_tx();
                });
                statistics_update = fk_uptime() + 1000;
            }

            fk_delay(10);
        }

        // NetworkTask dtor will fixup our state.
    }
}

}
