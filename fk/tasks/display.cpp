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

class ViewController {
public:
    virtual void show_menu() = 0;
    virtual void show_home() = 0;
    virtual void show_self_check() = 0;
    virtual void show_readings() = 0;
    virtual void show_name() = 0;

};

class DisplayView {
public:
    virtual void tick(ViewController *views) = 0;

public:
    virtual void show() {
    }

    virtual void up(ViewController *views) {
        views->show_home();
    }

    virtual void down(ViewController *views) {
        views->show_home();
    }

    virtual void enter(ViewController *views) {
        views->show_home();
    }

};

class HomeView : public DisplayView {
public:
    void tick(ViewController *views) override {
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

    void up(ViewController *views) override {
        views->show_menu();
    }

    void down(ViewController *views) override {
        views->show_menu();
    }

    void enter(ViewController *views) override {
        views->show_menu();
    }

};

class NameView : public DisplayView {
public:
    void tick(ViewController *views) override {
        SimpleScreen simple = { "Name" };

        auto bus = get_board()->i2c_core();
        auto display = get_display();
        display->simple(simple);
    }

};

class QrCodeView : public DisplayView {
public:
    void tick(ViewController *views) override {
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

};

class MenuView : public DisplayView {
private:
    MenuScreen *active_menu{ nullptr };
    MenuScreen *previous_menu{ nullptr };
    MenuScreen *info_menu;
    MenuScreen *network_menu;
    MenuScreen *tools_menu;
    MenuScreen *main_menu;
    uint32_t menu_time_{ 0 };

public:
    template<size_t N>
    MenuScreen *new_menu_screen(Pool &pool, MenuOption* (&&options)[N]) {
        auto n_options = (MenuOption **)pool.malloc(sizeof(MenuOption*) * (N + 1));
        for (size_t i = 0; i < N; ++i) {
            n_options[i] = options[i];
        }
        n_options[N] = nullptr;
        return new (pool) MenuScreen(n_options);
    }

public:
    MenuView(Pool &pool, ViewController *views) {
        auto back = to_lambda_option(pool, "Back", [=]() {
            // NOTE Fancy way of deselecting ourselves.
            // Could be selected from another back operation.
            for (auto i = 0; active_menu->options[i] != nullptr; ++i) {
                active_menu->options[i]->selected = false;
            }
            active_menu = goto_menu(previous_menu);
            previous_menu = nullptr;
        });

        auto tools_self_check = to_lambda_option(pool, "Self Check", [=]() {
            views->show_self_check();
        });
        auto tools_fsck = to_lambda_option(pool, "Run Fsck", [=]() {
            back->on_selected();
            views->show_home();
            auto worker = create_pool_wrapper<FsckWorker, DefaultWorkerPoolSize, PoolWorker<FsckWorker>>();
            if (!get_ipc()->launch_worker(worker)) {
                delete worker;
                return;
            }
        });
        auto tools_factory_reset = to_lambda_option(pool, "Factory Reset", [=]() {
            perform_factory_reset();
            back->on_selected();
            views->show_home();
        });
        tools_menu = new_menu_screen<4>(pool, {
            back,
            tools_self_check,
            tools_fsck,
            tools_factory_reset,
        });

        auto network_toggle = to_lambda_option(pool, "Toggle Wifi", [=]() {
            back->on_selected();
            views->show_home();
            auto worker = create_pool_wrapper<WifiToggleWorker, DefaultWorkerPoolSize, PoolWorker<WifiToggleWorker>>();
            if (!get_ipc()->launch_worker(worker)) {
                delete worker;
                return;
            }
        });
        network_menu = new_menu_screen<2>(pool, {
            back,
            network_toggle,
        });

        auto info_name = to_lambda_option(pool, "Name", [=]() {
            views->show_name();
            back->on_selected();
        });
        auto info_memory = to_lambda_option(pool, "Memory", [=]() {
            back->on_selected();
        });
        info_menu = new_menu_screen<3>(pool, {
            back,
            info_name,
            info_memory,
        });

        auto main_readings = to_lambda_option(pool, "Readings", [=]() {
            views->show_readings();
        });
        auto main_info = to_lambda_option(pool, "Info", [=]() {
            previous_menu = active_menu;
            active_menu = goto_menu(info_menu);
        });
        auto main_network = to_lambda_option(pool, "Network", [=]() {
            previous_menu = active_menu;
            active_menu = goto_menu(network_menu);
        });
        auto main_tools = to_lambda_option(pool, "Tools", [=]() {
            previous_menu = active_menu;
            active_menu = goto_menu(tools_menu);
        });

        main_menu = new_menu_screen<4>(pool, {
            main_readings,
            main_info,
            main_network,
            main_tools,
        });

        active_menu = main_menu;
    }

public:
    void show() override {
        menu_time_ = fk_uptime() + 5000;
    }

    void tick(ViewController *views) override {
        auto bus = get_board()->i2c_core();
        auto display = get_display();
        display->menu(*active_menu);

        if (fk_uptime() > menu_time_) {
            views->show_home();
        }
    }

    void up(ViewController *views) override {
        show();
        selection_up(*active_menu);
    }

    void down(ViewController *views) override {
        show();
        selection_down(*active_menu);
    }

    void enter(ViewController *views) override {
        show();
        selected(*active_menu)->on_selected();
    }

private:
    static void perform_factory_reset() {
        Storage storage{ MemoryFactory::get_data_memory() };
        FactoryWipe factory_wipe{ storage };
        if (factory_wipe.wipe()) {
            fk_delay(500);
            NVIC_SystemReset();
        }
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

};

class ReadingsView : public DisplayView {
public:
    void tick(ViewController *views) override {
        SimpleScreen simple = { "Readings" };

        auto bus = get_board()->i2c_core();
        auto display = get_display();
        display->simple(simple);
    }

};

class SelfCheckView : public DisplayView {
private: 
    DisplaySelfCheckCallbacks self_check_callbacks_;

public:
    void show() override {
        self_check_callbacks_.clear();
        auto worker = create_pool_wrapper<SelfCheckWorker, DefaultWorkerPoolSize, PoolWorker<SelfCheckWorker>>(self_check_callbacks_);
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    }

    void tick(ViewController *views) override {
        auto bus = get_board()->i2c_core();
        auto display = get_display();
        display->self_check(self_check_callbacks_.screen());
    }

};

class MainViewController : public ViewController {
private:
    HomeView home_view;
    ReadingsView readings_view;
    MenuView menu_view;
    NameView name_view;
    SelfCheckView self_check_view;
    DisplayView *view = &home_view;

public:
    MainViewController(Pool &pool) : menu_view{ pool, this } {
    }

public:
    void show_menu() override {
        view = &menu_view;
        view->show();
    }

    void show_home() override {
        view = &home_view;
        view->show();
    }

    void show_self_check() override {
        view = &self_check_view;
        view->show();
    }

    void show_readings() override {
        view = &readings_view;
        view->show();
    }

    void show_name() override {
        view = &name_view;
        view->show();
    }

    void run() {
        auto stop_time = fk_uptime() + fk_config().display.inactivity;

        while (fk_uptime() < stop_time) {
            Button *button = nullptr;
            if (get_ipc()->dequeue_button(&button)) {
                stop_time = fk_uptime() + fk_config().display.inactivity;

                switch (button->index()) {
                case Buttons::Left: {
                    loginfo("down");
                    view->down(this);
                    break;
                }
                case Buttons::Middle: {
                    loginfo("enter");
                    view->enter(this);
                    break;
                }
                case Buttons::Right: {
                    loginfo("up");
                    view->up(this);
                    break;
                }
                default: {
                    break;
                }
                }
            }

            view->tick(this);
        }
    }

};

void task_handler_display(void *params) {
    MallocPool pool{ "display", 4096 };
    MainViewController views{ pool };
    views.run();
    get_display()->off();
}

}
