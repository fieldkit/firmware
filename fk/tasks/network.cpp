#include <os.h>

#include "tasks/tasks.h"
#include "hal/hal.h"
#include "networking/server.h"
#include "state_manager.h"

namespace fk {

FK_DECLARE_LOGGER("network");

class SignalCheckCallback : public NetworkRunningCallback  {
private:
    bool signaled_{ false };

public:
    bool signaled() {
        return signaled_;
    }

    bool running() override {
        uint32_t signal = 0;
        if (os_signal_check(&signal) == OSS_SUCCESS) {
            if (signal > 0) {
                signaled_ = true;
            }
        }
        return !signaled_;
    }
};

void task_handler_network(void *params) {
    auto &fkc = fk_config();
    auto network = get_network();
    auto http_server = HttpServer{ network, &fkc };

    GlobalStateManager gsm;
    gsm.apply([=](GlobalState *gs) {
        gs->network.enabled = fk_uptime();
        gs->network.ip = get_network()->ip_address();
    });

    SignalCheckCallback signal_check;
    if (!http_server.begin(fkc.network.uptime, &signal_check)) {
        if (signal_check.signaled()) {
            return;
        }
        logerror("error starting server");
        return;
    }

    gsm.apply([=](GlobalState *gs) {
        gs->network.enabled = fk_uptime();
        gs->network.ip = get_network()->ip_address();
    });

    while (http_server.active_connections() || fk_uptime() - http_server.activity() < fkc.network.uptime) {
        http_server.tick();
        fk_delay(10);

        uint32_t signal = 0;
        if (os_signal_check(&signal) == OSS_SUCCESS) {
            if (signal > 0) {
                break;
            }
        }
    }

    http_server.stop();

    gsm.apply([=](GlobalState *gs) {
        gs->network = { };
    });

    loginfo("network stopped");
}

}
