#include "menu_view.h"
#include "simple_workers.h"
#include "configure_module_worker.h"
#include "factory_wipe.h"
#include "hal/board.h"
#include "state_ref.h"

namespace fk {

FK_DECLARE_LOGGER("menu");

template<typename T>
LambdaOption<T> *to_lambda_option(Pool *pool, const char *label, T fn) {
    return new (*pool) LambdaOption<T>(label, fn);
}

template<size_t N>
MenuScreen *new_menu_screen(Pool *pool, MenuOption* (&&options)[N]) {
    auto n_options = (MenuOption **)pool->malloc(sizeof(MenuOption*) * (N + 1));
    for (size_t i = 0; i < N; ++i) {
        n_options[i] = options[i];
    }
    n_options[N] = nullptr;
    return new (*pool) MenuScreen(n_options);
}

template<typename T>
struct NetworkOption : public MenuOption {
    WifiNetworkInfo network_;
    T fn_;

    NetworkOption(WifiNetworkInfo network, T fn) : MenuOption(""), network_(network), fn_(fn) {
    }

    void on_selected() override {
        fn_(network_);
    }

    bool visible() override {
        return network_.ssid[0] != 0;
    }

    const char *label() override {
        return network_.ssid;
    }
};

template<typename T>
NetworkOption<T> *to_network_option(Pool *pool, WifiNetworkInfo network, T fn) {
    return new (*pool) NetworkOption<T>(network, fn);
}

MenuView::MenuView(ViewController *views, Pool &pool) : pool_(&pool), views_(views) {
    back_ = to_lambda_option(&pool, "Back", [=]() {
        // NOTE Fancy way of deselecting ourselves.
        // Could be selected from another back operation.
        for (auto i = 0; active_menu_->options[i] != nullptr; ++i) {
            active_menu_->options[i]->selected_ = false;
        }
        if (previous_menu_ == nullptr) {
            active_menu_ = goto_menu(main_menu_);
        }
        else {
            active_menu_ = goto_menu(previous_menu_);
        }
    });

    create_info_menu();
    create_tools_menu();
    create_modules_menu();
    create_network_menu();
    create_main_menu();

    active_menu_ = main_menu_;
}

void MenuView::create_info_menu() {
    auto info_name = to_lambda_option(pool_, "Name", [=]() {
        views_->show_name();
        back_->on_selected();
    });
    auto info_memory = to_lambda_option(pool_, "Memory", [=]() {
        back_->on_selected();
    });
    auto info_modules = to_lambda_option(pool_, "Modules", [=]() {
        previous_menu_ = active_menu_;
        active_menu_ = goto_menu(modules_menu_);
    });

    info_menu_ = new_menu_screen<4>(pool_, {
        back_,
        info_name,
        info_memory,
        info_modules,
    });
}

void MenuView::create_modules_menu() {
    auto modules_water = to_lambda_option(pool_, "Water", [=]() {
        back_->on_selected();
        views_->show_home();
        auto worker = create_default_pool_worker<ConfigureModuleWorker>(ConfigureModuleKind::Water);
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    });
    auto modules_weather = to_lambda_option(pool_, "Weather", [=]() {
        back_->on_selected();
        views_->show_home();
        auto worker = create_default_pool_worker<ConfigureModuleWorker>(ConfigureModuleKind::Weather);
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    });
    auto modules_ultrasonic = to_lambda_option(pool_, "Ultrasonic", [=]() {
        back_->on_selected();
        views_->show_home();
        auto worker = create_default_pool_worker<ConfigureModuleWorker>(ConfigureModuleKind::Ultrasonic);
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    });

    modules_menu_ = new_menu_screen<4>(pool_, {
        back_,
        modules_water,
        modules_weather,
        modules_ultrasonic,
    });
}

void MenuView::create_tools_menu() {
    auto tools_self_check = to_lambda_option(pool_, "Self Check", [=]() {
        views_->show_self_check();
    });
    auto tools_fsck = to_lambda_option(pool_, "Run Fsck", [=]() {
        back_->on_selected();
        views_->show_home();
        auto worker = create_pool_wrapper<FsckWorker, DefaultWorkerPoolSize, PoolWorker<FsckWorker>>();
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    });
    auto tools_factory_reset = to_lambda_option(pool_, "Factory Reset", [=]() {
        get_display()->off();
        perform_factory_reset();
        back_->on_selected();
        views_->show_home();
    });

    tools_menu_ = new_menu_screen<4>(pool_, {
        back_,
        tools_self_check,
        tools_fsck,
        tools_factory_reset,
    });
}

void MenuView::create_network_menu() {
    auto gs = get_global_state_ro();

    MenuOption *network_options[MaximumNumberOfWifiNetworks];
    for (auto i = 0u; i < MaximumNumberOfWifiNetworks; ++i) {
        auto &n = gs.get()->network.config.wifi_networks[i];
        network_options[i] = to_network_option(pool_, n, [=](WifiNetworkInfo network) {
            choose_active_network(network);
            back_->on_selected();
            views_->show_home();
        });
    }
    auto network_choose_self = to_lambda_option(pool_, "Create AP", [=]() {
        choose_active_network({
            .valid = true,
            .create = true,
        });
        back_->on_selected();
        views_->show_home();
    });

    network_choose_menu_ = new_menu_screen<4>(pool_, {
        back_,
        network_choose_self,
        network_options[0],
        network_options[1],
    });


    auto network_toggle = to_lambda_option(pool_, "Toggle Wifi", [=]() {
        back_->on_selected();
        views_->show_home();
        auto worker = create_default_pool_worker<WifiToggleWorker>();
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    });
    auto network_choose = to_lambda_option(pool_, "Choose", [=]() {
        previous_menu_ = active_menu_;
        active_menu_ = goto_menu(network_choose_menu_);
    });

    network_menu_ = new_menu_screen<3>(pool_, {
        back_,
        network_toggle,
        network_choose,
    });
}

void MenuView::create_main_menu() {
    auto main_readings = to_lambda_option(pool_, "Readings", [=]() {
        views_->show_readings();
    });
    auto main_info = to_lambda_option(pool_, "Info", [=]() {
        previous_menu_ = active_menu_;
        active_menu_ = goto_menu(info_menu_);
    });
    auto main_network = to_lambda_option(pool_, "Network", [=]() {
        previous_menu_ = active_menu_;
        active_menu_ = goto_menu(network_menu_);
    });
    auto main_tools = to_lambda_option(pool_, "Tools", [=]() {
        previous_menu_ = active_menu_;
        active_menu_ = goto_menu(tools_menu_);
    });

    main_menu_ = new_menu_screen<4>(pool_, {
        main_readings,
        main_info,
        main_network,
        main_tools,
    });
}

void MenuView::show() {
    menu_time_ = fk_uptime() + 5000;
}

void MenuView::tick(ViewController *views) {
    auto bus = get_board()->i2c_core();
    auto display = get_display();
    display->menu(*active_menu_);

    if (fk_uptime() > menu_time_) {
        views->show_home();
    }
}

void MenuView::up(ViewController *views) {
    show();
    selection_up(*active_menu_);
}

void MenuView::down(ViewController *views) {
    show();
    selection_down(*active_menu_);
}

void MenuView::enter(ViewController *views) {
    show();
    selected(*active_menu_)->on_selected();
}

void MenuView::choose_active_network(WifiNetworkInfo network) {
    auto gs = get_global_state_rw();
    network.modified = fk_uptime();
    gs.get()->network.config.selected = network;
}

void MenuView::perform_factory_reset() {
    Storage storage{ MemoryFactory::get_data_memory() };
    FactoryWipe factory_wipe{ storage };
    if (factory_wipe.wipe()) {
        fk_delay(500);
        fk_restart();
    }
}

void MenuView::selection_up(MenuScreen &screen) {
    bool select_last = false;
    for (auto i = 0; screen.options[i] != nullptr; ++i) {
        if (screen.options[i]->selected()) {
            screen.options[i]->selected(false);
            if (i == 0) {
                select_last = true;
            }
            else {
                screen.options[i - 1]->selected(true);
                break;
            }
        }
        if (select_last) {
            if (screen.options[i + 1] == nullptr) {
                screen.options[i]->selected(true);
                break;
            }
        }
    }
}

void MenuView::selection_down(MenuScreen &screen) {
    for (auto i = 0; screen.options[i] != nullptr; ++i) {
        if (screen.options[i]->selected()) {
            screen.options[i]->selected(false);
            if (screen.options[i + 1] == nullptr) {
                screen.options[0]->selected(true);
            }
            else {
                screen.options[i + 1]->selected(true);
            }
            break;
        }
    }
}

MenuOption *MenuView::selected(MenuScreen &screen) {
    for (auto i = 0; screen.options[i] != nullptr; ++i) {
        if (screen.options[i]->selected()) {
            return screen.options[i];
        }
    }
    FK_ASSERT(0);
    return nullptr;
}

MenuScreen *MenuView::goto_menu(MenuScreen *screen) {
    MenuOption *selectable = nullptr;
    for (auto i = 0; screen->options[i] != nullptr; ++i) {
        if (screen->options[i]->selected()) {
            return screen;
        }
        if (selectable == nullptr) {
            selectable = screen->options[i];
        }
    }

    selectable->selected(true);

    return screen;
}

}
