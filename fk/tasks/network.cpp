#include <os.h>

#include "hal/hal.h"
#include "tasks/tasks.h"
#include "state_manager.h"
#include "networking/network_services.h"
#include "networking/network_duration.h"

namespace fk {

FK_DECLARE_LOGGER("network");

void try_and_serve_connections() {
    while (true) {
        auto network = get_network();

        StandardPool pool{ "network-task" };
        NetworkServices services{ network };
        NetworkDuration duration;
        GlobalStateManager gsm;

        auto settings = services.get_selected_settings(pool);

        gsm.apply([=](GlobalState *gs) {
            gs->network.state = { };
        });

        loginfo("starting network...");

        if (!services.begin(settings, NetworkConnectionTimeoutMs, pool)) {
            logerror("error starting server");
            return;
        }

        gsm.apply([&](GlobalState *gs) {
            strncpy(gs->network.state.ssid, services.ssid(), sizeof(gs->network.state.ssid));
            gs->network.state.ip = get_network()->ip_address();
            gs->network.state.enabled = fk_uptime();
            gs->network.state.connected = 0;
            duration = gs->scheduler.network.duration;
        });

        // In self AP mode we're waiting for connections now, and hold off doing
        // anything useful until something joins.
        uint32_t signal_checked = 0u;
        uint32_t started = fk_uptime();
        auto retry = false;
        while (!services.ready_to_serve()) {
            if (!duration.on(started)) {
                return;
            }

            // Some other task has requested that we stop serving. Menu option
            // or a self check for example.
            if (fk_task_stop_requested(&signal_checked)) {
                loginfo("stop requested");
                return;
            }

            if (services.did_configuration_change()) {
                loginfo("configuration change");
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
        if (!services.serve()) {
            logerror("error serving");
            continue;
        }

        gsm.apply([=](GlobalState *gs) {
            gs->network.state.enabled = fk_uptime();
            gs->network.state.connected = fk_uptime();
            gs->network.state.ip = get_network()->ip_address();
        });

        loginfo("awaiting connections...");

        auto statistics_update = fk_uptime() + OneSecondMs;

        while (true) {
            services.tick();

            // Break this loop and go to the beginning to recreate.
            if (services.did_configuration_change()) {
                loginfo("stopping: configuration");
                break;
            }

            // Some other task has requested that we stop serving. Menu option
            // or a self check for example.
            if (fk_task_stop_requested(&signal_checked)) {
                loginfo("stopping: killed");
                return;
            }

            // Check to see if we've been inactive for too long.
            if (!duration.on(services.activity())) {
                loginfo("stopping: inactive");
                return;
            }

            // This will happen when a foreign device disconnects from
            // our WiFi AP and in that case we keep the WiFi on until
            // we're properly considered inactive.
            if (!services.ready_to_serve()) {
                if (duration.always_on()) {
                    loginfo("stopping: disconnected (always-on)");
                    break;
                }
                else {
                    loginfo("stopping: disconnected");
                    break;
                }
            }

            if (fk_uptime() > statistics_update) {
                gsm.apply([&](GlobalState *gs) {
                    gs->network.state.bytes_rx = services.bytes_rx();
                    gs->network.state.bytes_tx = services.bytes_tx();
                    gs->network.state.activity = services.activity();
                    duration = gs->scheduler.network.duration;
                });
                statistics_update = fk_uptime() + OneSecondMs;
            }

            if (!services.active_connections()) {
                fk_delay(10);
            }
        }
    }
}

void task_handler_network(void *params) {
    try_and_serve_connections();

    GlobalStateManager gsm;
    gsm.apply([=](GlobalState *gs) {
        gs->network.state.ssid[0] = 0;
        gs->network.state.enabled = 0;
        gs->network.state.connected = 0;
        gs->network.state.activity = 0;
        gs->network.state.ip = 0;
    });
}

} // namespace fk
