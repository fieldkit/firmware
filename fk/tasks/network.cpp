#include <os.h>

#include "tasks/tasks.h"
#include "hal/hal.h"
#include "networking/server.h"
#include "state_manager.h"

namespace fk {

FK_DECLARE_LOGGER("network");

class NetworkTask {
private:
    Network *network_;
    HttpServer *http_server_;

public:
    NetworkTask(Network *network, HttpServer &http_server);
    virtual ~NetworkTask();

};

NetworkTask::NetworkTask(Network *network, HttpServer &http_server) : network_(network), http_server_(&http_server) {
}

NetworkTask::~NetworkTask() {
    loginfo("network stopping...");

    network_->stop();

    GlobalStateManager gsm;
    gsm.apply([=](GlobalState *gs) {
        gs->network.state = { };
    });

    loginfo("network stopped");
}

void task_handler_network(void *params) {
    while (true) {
        auto &fkc = fk_config();
        auto network = get_network();

        GlobalStateManager gsm;
        MallocPool pool{ "task:network", 256 };
        HttpServer http_server{ network, &fkc };
        NetworkTask task{ network, http_server };

        gsm.apply([=](GlobalState *gs) {
            gs->network.state = { };
        });

        // Either create a new AP or try and join an existing one.
        if (!http_server.begin(WifiConnectionTimeoutMs, pool)) {
            logerror("error starting server");
            return;
        }

        gsm.apply([=](GlobalState *gs) {
            gs->network.state.ip = get_network()->ip_address();
            gs->network.state.enabled = fk_uptime();
            gs->network.state.connected = 0;
        });

        // In self AP mode we're waiting for connections now, and hold off doing
        // anything useful until something joins.
        auto started = fk_uptime();
        while (!http_server.ready_to_serve()) {
            if (fk_uptime() - started > fkc.network.uptime) {
                return;
            }
            fk_delay(10);
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

            fk_delay(10);
        }

        // NetworkTask dtor will fixup our state.
    }
}

}
