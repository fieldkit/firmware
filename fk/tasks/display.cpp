#include "tasks/tasks.h"
#include "hal/metal/metal.h"

namespace fk {

FK_DECLARE_LOGGER("display");

static void show_home() {
    auto bus = get_board()->i2c_core();
    auto display = get_display();

    HomeScreen screen = {
        .time = fk_uptime(),
        .wifi = true,
        .gps = true,
        .battery = 1.0f,
        .message = "Hello",
    };

    display->home(screen);
}

static void show_menu(MenuScreen const &screen) {
    auto bus = get_board()->i2c_core();
    auto display = get_display();

    display->menu(screen);
}

void task_handler_display(void *params) {
    auto stop_time = fk_uptime() + fk_config().display.inactivity;
    auto menu_time = (uint32_t)0;

    MenuScreen menu;

    while (fk_uptime() < stop_time) {
        if (menu_time > 0) {
            show_menu(menu);

            if (fk_uptime() > menu_time) {
                menu_time = 0;
                menu = MenuScreen{ };
            }
        }
        else {
            show_home();
        }

        Button *button = nullptr;
        if (get_ipc()->dequeue_button(&button)) {
            stop_time = fk_uptime() + fk_config().display.inactivity;
            menu_time = fk_uptime() + 5000;

            loginfo("button!");

            get_ipc()->enqueue_data([](GlobalState *gs) {
                gs->runtime.activity = fk_uptime();
            });
        }
    }

    get_display()->off();
}

}
