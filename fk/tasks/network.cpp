#include "tasks/tasks.h"

#include "hal/hal.h"
#include "httpd/server.h"

#include "secrets.h"

namespace fk {

FK_DECLARE_LOGGER("network");

void task_handler_network(void *params) {
    auto network = get_network();
    #if defined(FK_WIFI_0_SSID) && defined(FK_WIFI_0_PASSWORD)
    HttpServer http_server{ network, FK_WIFI_0_SSID, FK_WIFI_0_PASSWORD };
    #else
    HttpServer http_server{ network };
    #endif

    if (!http_server.begin()) {
        logerror("error starting server");
        return;
    }

    while (http_server.active_connections() || fk_uptime() - http_server.activity() < fkc.network.uptime) {
        http_server.tick();
        fk_delay(10);
    }

    http_server.stop();

    loginfo("network stopped");
}

}
