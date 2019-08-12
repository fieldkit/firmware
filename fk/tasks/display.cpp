#include "tasks/tasks.h"
#include "hal/metal/metal.h"
#include "pool.h"

#include <qrcode.h>

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

static void show_qr() {
    auto bus = get_board()->i2c_core();
    auto display = get_display();

    QRCode qr;
    uint8_t version = 3;
    uint8_t data[qrcode_getBufferSize(version)];
    qrcode_initText(&qr, data, version, ECC_LOW, "HELLO WORLD");

    QrCodeScreen screen;
    screen.size = qr.size;
    screen.data = data;
    display->qr(screen);
}

static void selection_up(MenuScreen &screen) {
    bool select_last = false;
    for (auto i = 0; screen.options[i] != nullptr; ++i) {
        if (screen.options[i]->selected) {
            screen.options[i]->selected = false;
            if (i == 0) {
                select_last = true;
            }
            else {
                screen.options[i - 1]->selected = true;
                break;
            }
        }
        if (select_last) {
            if (screen.options[i + 1] == nullptr) {
                screen.options[i]->selected = true;
                break;
            }
        }
    }
}

static void selection_down(MenuScreen &screen) {
    for (auto i = 0; screen.options[i] != nullptr; ++i) {
        if (screen.options[i]->selected) {
            screen.options[i]->selected = false;
            if (screen.options[i + 1] == nullptr) {
                screen.options[0]->selected = true;
            }
            else {
                screen.options[i + 1]->selected = true;
            }
            break;
        }
    }
}

static MenuOption *selected(MenuScreen &screen) {
    for (auto i = 0; screen.options[i] != nullptr; ++i) {
        if (screen.options[i]->selected) {
            return screen.options[i];
        }
    }
    FK_ASSERT(0);
    return nullptr;
}

void task_handler_display(void *params) {
    auto stop_time = fk_uptime() + fk_config().display.inactivity;
    auto menu_time = (uint32_t)0;

    auto data = to_lambda_option("Data", []() {
        loginfo("data");
    });
    auto configure = to_lambda_option("Configure", []() {
        loginfo("configure");
    });
    auto qr_code = to_lambda_option("QR", []() {
        loginfo("qr");
    });
    auto info = to_lambda_option("Info", []() {
        loginfo("info");
    });

    MenuOption *options[] = {
        &data,
        &configure,
        &qr_code,
        &info,
        nullptr,
    };
    MenuScreen menu{ (MenuOption **)&options };

    options[0]->selected = true;

    while (fk_uptime() < stop_time) {
        if (menu_time > 0) {
            show_menu(menu);

            if (fk_uptime() > menu_time) {
                menu_time = 0;
            }
        }
        else {
            if (false) {
                show_home();
            }
            else {
                show_qr();
            }
        }

        Button *button = nullptr;
        if (get_ipc()->dequeue_button(&button)) {
            stop_time = fk_uptime() + fk_config().display.inactivity;
            menu_time = fk_uptime() + 5000;

            switch (button->index()) {
            case Buttons::Left: {
                loginfo("down");
                selection_down(menu);
                break;
            }
            case Buttons::Middle: {
                loginfo("enter");
                selected(menu)->on_selected();
                break;
            }
            case Buttons::Right: {
                loginfo("up");
                selection_up(menu);
                break;
            }
            default: {
                break;
            }
            }

            get_ipc()->enqueue_data([](GlobalState *gs) {
                gs->runtime.activity = fk_uptime();
            });
        }
    }

    get_display()->off();
}

}
