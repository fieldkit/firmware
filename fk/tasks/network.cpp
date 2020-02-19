#include <os.h>

#include "tasks/tasks.h"
#include "hal/hal.h"
#include "networking/network_task.h"

namespace fk {

FK_DECLARE_LOGGER("network");

struct NetworkDuration {
    uint32_t ms{ FiveMinutesMs };

    bool always_on() const {
        return ms == UINT32_MAX;
    }

    bool on(uint32_t activity) const {
        return always_on() || (fk_uptime() - activity) < ms;
    }

    NetworkDuration operator=(uint32_t ms) {
        this->ms = ms;
        return *this;
    }
};

void task_handler_network(void *params) {
    while (true) {
        auto network = get_network();

        GlobalStateManager gsm;
        NetworkServices network_services{ network };
        NetworkTask task{ network, network_services };
        StandardPool pool{ "network" };
        NetworkDuration duration;

        auto settings = task.get_selected_settings(pool);

        gsm.apply([=](GlobalState *gs) {
            gs->network.state = { };
        });

        loginfo("starting network...");

        if (!task.begin(settings, NetworkConnectionTimeoutMs, pool)) {
            logerror("error starting server");
            return;
        }

        gsm.apply([&](GlobalState *gs) {
            strncpy(gs->network.state.ssid, network_services.ssid(), sizeof(gs->network.state.ssid));
            gs->network.state.ip = get_network()->ip_address();
            gs->network.state.enabled = fk_uptime();
            gs->network.state.connected = 0;
            duration = gs->scheduler.network.duration;
        });

        // In self AP mode we're waiting for connections now, and hold off doing
        // anything useful until something joins.
        auto started = fk_uptime();
        auto retry = false;
        while (!network_services.ready_to_serve()) {
            if (!duration.on(started)) {
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
        if (!network_services.serve()) {
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
            network_services.tick();

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
            if (!duration.on(network_services.activity())) {
                loginfo("inactive");
                return;
            }

            // This will happen when a foreign device disconnects from our WiFi AP.
            if (!network_services.ready_to_serve()) {
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
                    gs->network.state.bytes_rx = network_services.bytes_rx();
                    gs->network.state.bytes_tx = network_services.bytes_tx();
                    duration = gs->scheduler.network.duration;
                });
                statistics_update = fk_uptime() + OneSecondMs;
            }

            fk_delay(10);
        }
    }
}

}
