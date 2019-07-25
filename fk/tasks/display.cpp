#include "tasks/tasks.h"

#include "hal/metal/metal.h"

namespace fk {

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
    while (true) {
        refresh();

        fk_delay(1000);
    }
}

}
