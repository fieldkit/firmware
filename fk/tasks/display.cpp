#include "tasks/tasks.h"
#include "hal/metal/metal.h"
#include "pool.h"

#include "simple_workers.h"

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

    fk_serial_number_t sn;
    fk_serial_number_get(&sn);

    // Version 3 (29x29) LOW ECC
    // Numeric = 127
    // Alphanumeric = 77
    // Bytes = 53
    QRCode qr;
    uint8_t version = 3;
    uint8_t data[qrcode_getBufferSize(version)];
    qrcode_initBytes(&qr, data, version, ECC_LOW, (uint8_t *)&sn, sizeof(sn));

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

static MenuScreen *goto_menu(MenuScreen *screen) {
    MenuOption *selectable = nullptr;
    for (auto i = 0; screen->options[i] != nullptr; ++i) {
        if (screen->options[i]->selected) {
            loginfo("already selected %d '%s'", i, screen->options[i]->label);
            return screen;
        }
        if (selectable == nullptr) {
            selectable = screen->options[i];
        }
    }

    loginfo("selecting %s", selectable->label);
    selectable->selected = true;

    return screen;
}

void task_handler_display(void *params) {
    auto stop_time = fk_uptime() + fk_config().display.inactivity;
    auto menu_time = (uint32_t)0;
    MenuScreen *active_menu{ nullptr };
    MenuScreen *previous_menu{ nullptr };

    auto back = to_lambda_option("Back", [&]() {
        // Fancy way of deselecting ourselves.
        for (auto i = 0; active_menu->options[i] != nullptr; ++i) {
            active_menu->options[i]->selected = false;
        }
        active_menu = goto_menu(previous_menu);
        previous_menu = nullptr;
        // TODO: Could be selected from another back operation.
    });

    auto fsck = to_lambda_option("Run fsck", [&]() {
        // TODO: MALLOC
        if (!get_ipc()->launch_worker(new FsckWorker())) {
            return;
        }
        back.on_selected();
    });
    MenuOption *tools_options[] = {
        &back,
        &fsck,
        nullptr,
    };
    MenuScreen tools_menu{ (MenuOption **)&tools_options };

    auto wifi_toggle = to_lambda_option("Toggle Wifi", [&]() {
        // TODO: Remember this and skip restarting network.
        // TODO: MALLOC
        if (!get_ipc()->launch_worker(new WifiToggleWorker())) {
            return;
        }
        back.on_selected();
    });
    MenuOption *network_options[] = {
        &back,
        &wifi_toggle,
        nullptr,
    };
    MenuScreen network_menu{ (MenuOption **)&network_options };

    auto readings = to_lambda_option("Readings", []() {
        loginfo("readings");
    });
    auto memory = to_lambda_option("Memory", []() {
        loginfo("memory");
    });
    auto network = to_lambda_option("Network", [&]() {
        previous_menu = active_menu;
        active_menu = goto_menu(&network_menu);
    });
    auto tools = to_lambda_option("Tools", [&]() {
        previous_menu = active_menu;
        active_menu = goto_menu(&tools_menu);
    });

    MenuOption *main_options[] = {
        &readings,
        &memory,
        &network,
        &tools,
        nullptr,
    };
    MenuScreen main_menu{ (MenuOption **)&main_options };

    active_menu = &main_menu;

    while (fk_uptime() < stop_time) {
        Button *button = nullptr;
        if (get_ipc()->dequeue_button(&button)) {
            stop_time = fk_uptime() + fk_config().display.inactivity;
            menu_time = fk_uptime() + 5000;

            switch (button->index()) {
            case Buttons::Left: {
                loginfo("down");
                selection_down(*active_menu);
                break;
            }
            case Buttons::Middle: {
                loginfo("enter");
                auto s = selected(*active_menu);
                s->on_selected();
                break;
            }
            case Buttons::Right: {
                loginfo("up");
                selection_up(*active_menu);
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
        if (menu_time > 0) {
            show_menu(*active_menu);

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
    }

    get_display()->off();
}

}
