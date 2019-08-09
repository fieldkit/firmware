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
    auto started = fk_uptime();

    while (fk_uptime() - started < fk_config().display.inactivity) {
        refresh();

        Button *button = nullptr;
        if (get_ipc()->dequeue_button(&button)) {
            loginfo("button!");
        }
    }

    get_display()->off();
}

}
