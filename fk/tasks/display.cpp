#include "tasks/tasks.h"

#include "hal/metal/metal.h"

namespace fk {

void task_handler_display(void *params) {
    auto display = get_display();

    while (true) {
        home_screen_t screen = {
            .time = fk_uptime(),
            .wifi = true,
            .gps = true,
            .battery = 1.0f,
        };

        display->home(screen);

        fk_delay(1000);
    }
}

}
