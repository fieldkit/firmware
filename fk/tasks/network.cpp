#include <os.h>

#include "hal/hal.h"
#include "tasks/tasks.h"
#include "state_manager.h"
#include "networking/network_services.h"
#include "timer.h"

namespace fk {

FK_DECLARE_LOGGER("network");

void try_and_serve_connections() {
    while (true) {
        StandardPool pool{ "network-task" };
        NetworkServices services{ get_network() };
        GlobalStateManager gsm;

        gsm.apply([=](GlobalState *gs) {
            gs->network.state = { };
        });

        loginfo("starting network...");

        if (!services.begin(NetworkConnectionTimeoutMs, pool)) {
            logerror("error starting server");
            return;
        }

        gsm.apply([&](GlobalState *gs) {
            strncpy(gs->network.state.ssid, services.ssid(), sizeof(gs->network.state.ssid));
            gs->network.state.enabled = fk_uptime();
            gs->network.state.connected = 0;
            gs->network.state.ip = get_network()->ip_address();
        });

        // In self AP mode we're waiting for connections now, and hold off doing
        // anything useful until something joins.
        while (services.waiting_to_serve()) {
            if (services.should_stop()) {
                return;
            }

            services.tick();

            fk_delay(10);
        }

        if (!services.can_serve()) {
            loginfo("unable to serve, retrying");
            continue;
        }

        // Start the network services now that we've got things to talk to.
        if (!services.serve()) {
            logerror("error serving");
            continue;
        }

        loginfo("awaiting connections...");

        gsm.apply([=](GlobalState *gs) {
            gs->network.state.enabled = fk_uptime();
            gs->network.state.connected = fk_uptime();
            gs->network.state.ip = get_network()->ip_address();
        });

        IntervalTimer refresh_statistics{ OneSecondMs };
        while (services.serving()) {
            if (services.should_stop()) {
                return;
            }

            services.tick();

            if (!services.active_connections()) {
                fk_delay(10);
            }

            if (refresh_statistics.expired()) {
                gsm.apply([&](GlobalState *gs) {
                    gs->network.state.bytes_rx = services.bytes_rx();
                    gs->network.state.bytes_tx = services.bytes_tx();
                    gs->network.state.activity = services.activity();
                });
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
