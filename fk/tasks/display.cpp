#include "tasks/tasks.h"

#include "hal/metal/metal.h"

namespace fk {

void task_handler_display(void *params) {
    DisplayFactory display_factory;
    Display *display = display_factory.get_display();

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
