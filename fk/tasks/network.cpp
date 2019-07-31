#include "tasks/tasks.h"
#include "hal/hal.h"
#include "networking/server.h"
#include "state.h"

namespace fk {

FK_DECLARE_LOGGER("network");

void task_handler_network(void *params) {
    auto &fkc = fk_config();
    auto network = get_network();
    auto http_server = HttpServer{ network, &fkc };

    if (!http_server.begin()) {
        logerror("error starting server");
        return;
    }

    get_ipc()->enqueue_data([](GlobalState *gs) {
        gs->network.enabled = fk_uptime();
        gs->network.ip = get_network()->ip_address();
    });

    while (http_server.active_connections() || fk_uptime() - http_server.activity() < fkc.network.uptime) {
        http_server.tick();
        fk_delay(10);
    }

    http_server.stop();

    get_ipc()->enqueue_data([](GlobalState *gs) {
        gs->network = { };
    });

    loginfo("network stopped");
}

}
