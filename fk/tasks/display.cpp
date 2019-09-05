#include "tasks/tasks.h"
#include "hal/metal/metal.h"
#include "pool.h"
#include "state_manager.h"
#include "simple_workers.h"
#include "display_self_check_callbacks.h"
#include "storage/storage.h"
#include "factory_wipe.h"

#include <qrcode.h>

namespace fk {

FK_DECLARE_LOGGER("display");

static void show_home() {
    auto bus = get_board()->i2c_core();
    auto display = get_display();

    auto gs = get_global_state_ro();

    HomeScreen screen;
    screen.time = fk_uptime();
    screen.recording = gs.get()->general.recording;
    screen.network = gs.get()->network.enabled;
    screen.gps = gs.get()->gps.fix;
    screen.battery = gs.get()->power.charge;
    screen.message = gs.get()->general.name;
    screen.logo = true;
    screen.progress = {
        gs.get()->progress.operation,
        gs.get()->progress.progress,
    };

    display->home(screen);
}

static void show_menu(MenuScreen const &screen) {
    auto bus = get_board()->i2c_core();
    auto display = get_display();

    display->menu(screen);
}

static void show_self_check_screen(SelfCheckScreen &screen) {
    auto bus = get_board()->i2c_core();
    auto display = get_display();

    display->self_check(screen);
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

static void perform_factory_reset() {
    Storage storage{ MemoryFactory::get_data_memory() };
    FactoryWipe factory_wipe{ storage };
    if (factory_wipe.wipe()) {
        fk_delay(500);
        NVIC_SystemReset();
    }
}

// TODO Move this into a class with state, like previous_menu.
// TODO Collapse options and their menu into a single templatized class?
void task_handler_display(void *params) {
    auto stop_time = fk_uptime() + fk_config().display.inactivity;
    auto menu_time = (uint32_t)0;
    MenuScreen *active_menu{ nullptr };
    MenuScreen *previous_menu{ nullptr };
    DisplayScreen *active_screen{ nullptr };
    DisplaySelfCheckCallbacks self_check_callbacks;
    SimpleScreen simple{ "" };

    auto back = to_lambda_option("Back", [&]() {
        // NOTE Fancy way of deselecting ourselves.
        // Could be selected from another back operation.
        for (auto i = 0; active_menu->options[i] != nullptr; ++i) {
            active_menu->options[i]->selected = false;
        }
        active_menu = goto_menu(previous_menu);
        previous_menu = nullptr;
    });


    auto tools_self_check = to_lambda_option("Self Check", [&]() {
        self_check_callbacks.clear();
        active_screen = &self_check_callbacks.screen();
        menu_time = 0;
        auto worker = create_pool_wrapper<SelfCheckWorker, DefaultWorkerPoolSize, PoolWorker<SelfCheckWorker>>(self_check_callbacks);
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    });
    auto tools_fsck = to_lambda_option("Run Fsck", [&]() {
        back.on_selected();
        menu_time = 0;
        auto worker = create_pool_wrapper<FsckWorker, DefaultWorkerPoolSize, PoolWorker<FsckWorker>>();
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    });
    auto tools_factory_reset = to_lambda_option("Factory Reset", [&]() {
        perform_factory_reset();
        back.on_selected();
        menu_time = 0;
    });
    MenuOption *tools_options[] = {
        &back,
        &tools_self_check,
        &tools_fsck,
        &tools_factory_reset,
        nullptr,
    };
    MenuScreen tools_menu{ (MenuOption **)&tools_options };


    auto network_toggle = to_lambda_option("Toggle Wifi", [&]() {
        back.on_selected();
        menu_time = 0;
        auto worker = create_pool_wrapper<WifiToggleWorker, DefaultWorkerPoolSize, PoolWorker<WifiToggleWorker>>();
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    });
    MenuOption *network_options[] = {
        &back,
        &network_toggle,
        nullptr,
    };
    MenuScreen network_menu{ (MenuOption **)&network_options };

    auto info_name = to_lambda_option("Name", [&]() {
        simple = { "Name" };
        active_screen = &simple;
        back.on_selected();
    });
    auto info_memory = to_lambda_option("Memory", [&]() {
        simple = { "Memory" };
        active_screen = &simple;
        back.on_selected();
    });
    MenuOption *info_options[] = {
        &back,
        &info_name,
        &info_memory,
        nullptr,
    };
    MenuScreen info_menu{ (MenuOption **)&info_options };


    auto main_readings = to_lambda_option("Readings", []() {
        loginfo("readings");
    });
    auto main_info = to_lambda_option("Info", [&]() {
        previous_menu = active_menu;
        active_menu = goto_menu(&info_menu);
    });
    auto main_network = to_lambda_option("Network", [&]() {
        previous_menu = active_menu;
        active_menu = goto_menu(&network_menu);
    });
    auto main_tools = to_lambda_option("Tools", [&]() {
        previous_menu = active_menu;
        active_menu = goto_menu(&tools_menu);
    });

    MenuOption *main_options[] = {
        &main_readings,
        &main_info,
        &main_network,
        &main_tools,
        nullptr,
    };
    MenuScreen main_menu{ (MenuOption **)&main_options };

    active_menu = &main_menu;

    while (fk_uptime() < stop_time) {
        Button *button = nullptr;
        if (get_ipc()->dequeue_button(&button)) {
            stop_time = fk_uptime() + fk_config().display.inactivity;
            menu_time = fk_uptime() + 5000;

            if (active_screen != nullptr) {
                loginfo("clearing active screen");
                active_screen = nullptr;
            }

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
        }

        if (get_network()->enabled()) {
            network_toggle.label = "Stop WiFi";
        }
        else {
            network_toggle.label = "Start WiFi";
        }

        if (active_screen != nullptr) {
            if (active_screen == &self_check_callbacks.screen()) {
                show_self_check_screen(self_check_callbacks.screen());
            }
            if (active_screen == &simple) {
                auto bus = get_board()->i2c_core();
                auto display = get_display();
                display->simple(simple);
            }
        }
        else if (menu_time > 0) {
            show_menu(*active_menu);

            if (fk_uptime() > menu_time) {
                menu_time = 0;
            }
        }
        else {
            if (true) {
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
