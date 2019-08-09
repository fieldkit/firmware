#include "tasks/tasks.h"
#include "hal/metal/metal.h"

namespace fk {

FK_DECLARE_LOGGER("display");

static void refresh() {
    auto bus = get_board()->i2c_core();
    auto display = get_display();

    home_screen_t screen = {
        .time = fk_uptime(),
        .wifi = true,
        .gps = true,
        .battery = 1.0f,
    };

    display->home(screen);
}

void task_handler_display(void *params) {
    auto stop_time = fk_uptime() + fk_config().display.inactivity;

    while (stop_time < fk_uptime()) {
        refresh();

        Button *button = nullptr;
        if (get_ipc()->dequeue_button(&button)) {
            stop_time = fk_uptime() + fk_config().display.inactivity;

            loginfo("button!");

            get_ipc()->enqueue_data([](GlobalState *gs) {
                gs->runtime.activity = fk_uptime();
            });
        }
    }

    get_display()->off();
}

}
