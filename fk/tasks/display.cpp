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

    HomeScreen screen;
    screen.time = fk_uptime();
    screen.wifi = true;
    screen.gps = true;
    screen.battery = 1.0f;
    screen.message = "Hello";

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

class DisplaySelfCheckCallbacks : public SelfCheckCallbacks {
public:
    constexpr static size_t NumberOfChecks = 10;

private:
    SelfCheckScreen screen_;
    Check checks_[NumberOfChecks];
    Check *queued_[NumberOfChecks + 1] = { nullptr };
    size_t number_{ 0 };

public:
    DisplaySelfCheckCallbacks() {
        screen_.checks = queued_;
    }

public:
    void update(SelfCheckStatus status) override {
        number_ = 0;
        append("rtc", status.rtc);
        append("temp", status.temperature);
        append("bg", status.battery_gauge);
        append("qspi", status.qspi_memory);
        append("spi", status.spi_memory);
        append("wifi", status.wifi);
        append("sd", status.sd_card);
        append("bpm", status.bp_mux);
        append("bps", status.bp_shift);
    }

    void append(const char *name, CheckStatus status) {
        FK_ASSERT(number_ < NumberOfChecks);
        if (status == CheckStatus::Pass) {
            checks_[number_] = { name, true };
            queued_[number_] = &checks_[number_];
            number_++;
            queued_[number_] = nullptr;
        }
        else if (status == CheckStatus::Fail) {
            checks_[number_] = { name, false };
            queued_[number_] = &checks_[number_];
            number_++;
            queued_[number_] = nullptr;
        }
    }

    void clear() {
        queued_[0] = nullptr;
        number_ = 0;
    }

public:
    SelfCheckScreen &screen() {
        return screen_;
    }

};

void task_handler_display(void *params) {
    auto stop_time = fk_uptime() + fk_config().display.inactivity;
    auto menu_time = (uint32_t)0;
    MenuScreen *active_menu{ nullptr };
    MenuScreen *previous_menu{ nullptr };
    DisplayScreen *active_screen{ nullptr };
    DisplaySelfCheckCallbacks self_check_callbacks;

    auto back = to_lambda_option("Back", [&]() {
        // NOTE Fancy way of deselecting ourselves.
        // Could be selected from another back operation.
        for (auto i = 0; active_menu->options[i] != nullptr; ++i) {
            active_menu->options[i]->selected = false;
        }
        active_menu = goto_menu(previous_menu);
        previous_menu = nullptr;
    });

    auto self_check = to_lambda_option("Self Check", [&]() {
        self_check_callbacks.clear();
        active_screen = &self_check_callbacks.screen();
        auto worker = create_pool_worker<SelfCheckWorker>(DefaultWorkerPoolSize, &self_check_callbacks);
        if (!get_ipc()->launch_worker(worker)) {
            return;
        }
    });
    auto fsck = to_lambda_option("Run Fsck", [&]() {
        auto worker = create_pool_worker<FsckWorker>(DefaultWorkerPoolSize);
        if (!get_ipc()->launch_worker(worker)) {
            return;
        }
        back.on_selected();
    });
    MenuOption *tools_options[] = {
        &back,
        &self_check,
        &fsck,
        nullptr,
    };
    MenuScreen tools_menu{ (MenuOption **)&tools_options };

    auto wifi_toggle = to_lambda_option("Toggle Wifi", [&]() {
        // TODO: Remember this and skip restarting network.
        auto worker = create_pool_worker<WifiToggleWorker>(DefaultWorkerPoolSize);
        if (!get_ipc()->launch_worker(worker)) {
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
            wifi_toggle.label = "Stop WiFi";
        }
        else {
            wifi_toggle.label = "Start WiFi";
        }

        if (active_screen != nullptr) {
            if (active_screen == &self_check_callbacks.screen()) {
                show_self_check_screen(self_check_callbacks.screen());
            }
        }
        else if (menu_time > 0) {
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
