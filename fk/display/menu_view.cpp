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

    bool active() const override {
        return network_.ssid[0] != 0;
    }

    const char *label() const override {
        return network_.ssid;
    }
};

template<typename T>
NetworkOption<T> *to_network_option(Pool *pool, WifiNetworkInfo network, T fn) {
    return new (*pool) NetworkOption<T>(network, fn);
}

template<typename T>
struct ModuleBayOption : public MenuOption {
    uint8_t index_;
    T fn_;

    ModuleBayOption(uint8_t index, const char *label, T fn) : MenuOption(label), index_(index), fn_(fn) {
    }

    void on_selected() override {
        fn_();
    }

    bool active() const override {
        return true;
    }
};

template<typename T>
ModuleBayOption<T> *to_module_bay_option(Pool *pool, uint8_t index, const char *label, T fn) {
    return new (*pool) ModuleBayOption<T>(index, label, fn);
}

struct ToggleWifiOption : public MenuOption {
    MenuOption *back_;
    ViewController *views_;

    ToggleWifiOption(MenuOption *back, ViewController *views) : MenuOption(""), back_(back), views_(views) {
    }

    void on_selected() override {
        back_->on_selected();
        views_->show_home();
        auto worker = create_default_pool_worker<WifiToggleWorker>();
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    }

    const char *label() const override {
        if (get_network()->enabled()) {
            return "Disable";
        }
        return "Enable";
    }
};

MenuView::MenuView(ViewController *views, Pool &pool) : pool_(&pool), views_(views) {
    back_ = to_lambda_option(&pool, "Back", [=]() {
        // NOTE Fancy way of deselecting ourselves.
        // Could be selected from another back operation.
        for (auto i = 0; active_menu_->options[i] != nullptr; ++i) {
            active_menu_->options[i]->selected_ = false;
        }
        if (previous_menu_ == nullptr || previous_menu_ == active_menu_) {
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
    refresh_visible(*active_menu_, 0);
}

void MenuView::create_info_menu() {
    auto info_name = to_lambda_option(pool_, "Name", [=]() {
        views_->show_name();
        back_->on_selected();
    });
    auto info_memory = to_lambda_option(pool_, "Memory", [=]() {
        back_->on_selected();
    });
    auto info_build = to_lambda_option(pool_, "Build", [=]() {
        views_->show_build();
        back_->on_selected();
    });

    info_menu_ = new_menu_screen<4>(pool_, {
        back_,
        info_build,
        info_name,
        info_memory,
    });
}

void MenuView::create_modules_menu() {
    MenuOption *bay_options[5];
    for (auto i = 0u; i < 4u; ++i) {
        bay_options[i] = to_module_bay_option(pool_, i * 2, pool_->sprintf("%d", i + 1), [=]() {
            selected_module_bay_ = i * 2;
            active_menu_ = goto_menu(module_menu_);
            loginfo("selected %d", i);
        });
    }
    bay_options[4] = to_module_bay_option(pool_, AllModuleBays, "All", [=]() {
        selected_module_bay_ = AllModuleBays;
        active_menu_ = goto_menu(module_menu_);
        loginfo("selected all");
    });
    module_bays_menu_ = new_menu_screen<4>(pool_, {
        bay_options[0], bay_options[1], bay_options[2], bay_options[3],
    });


    auto modules_water = to_lambda_option(pool_, "Water", [=]() {
        back_->on_selected();
        views_->show_home();
        auto worker = create_default_pool_worker<ConfigureModuleWorker>(selected_module_bay_, ConfigureModuleKind::Water);
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    });
    auto modules_weather = to_lambda_option(pool_, "Weather", [=]() {
        back_->on_selected();
        views_->show_home();
        auto worker = create_default_pool_worker<ConfigureModuleWorker>(selected_module_bay_, ConfigureModuleKind::Weather);
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    });
    auto modules_ultrasonic = to_lambda_option(pool_, "Ultrasonic", [=]() {
        back_->on_selected();
        views_->show_home();
        auto worker = create_default_pool_worker<ConfigureModuleWorker>(selected_module_bay_, ConfigureModuleKind::Ultrasonic);
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    });

    module_menu_ = new_menu_screen<4>(pool_, {
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
    auto tools_restart = to_lambda_option(pool_, "Restart", [=]() {
        get_display()->off();
        fk_restart();
    });

    tools_menu_ = new_menu_screen<5>(pool_, {
        back_,
        tools_self_check,
        tools_fsck,
        tools_restart,
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
        choose_active_network({ true, true });
        back_->on_selected();
        views_->show_home();
    });

    network_choose_menu_ = new_menu_screen<4>(pool_, {
        back_,
        network_choose_self,
        network_options[0],
        network_options[1],
    });


    auto network_toggle = new (*pool_) ToggleWifiOption(back_, views_);
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
    auto main_modules = to_lambda_option(pool_, "Modules", [=]() {
        previous_menu_ = active_menu_;
        active_menu_ = goto_menu(module_bays_menu_);
    });
    auto main_tools = to_lambda_option(pool_, "Tools", [=]() {
        previous_menu_ = active_menu_;
        active_menu_ = goto_menu(tools_menu_);
    });

    main_menu_ = new_menu_screen<5>(pool_, {
        main_readings,
        main_info,
        main_network,
        main_modules,
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

    if (!get_network()->enabled()) {
        auto worker = create_default_pool_worker<WifiToggleWorker>();
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    }
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
    auto select_last = false;
    auto previous_selectable_index = -1;

    for (auto i = 0; screen.options[i] != nullptr; ++i) {
        if (screen.options[i]->active()) {
            if (screen.options[i]->selected()) {
                screen.options[i]->selected(false);

                if (previous_selectable_index == -1) {
                    select_last = true;
                }
                else {
                    screen.options[previous_selectable_index]->selected(true);
                    refresh_visible(screen, previous_selectable_index);
                    break;
                }
            }

            previous_selectable_index = i;
        }
    }

    if (select_last) {
        FK_ASSERT(previous_selectable_index >= 0);
        auto &option = screen.options[previous_selectable_index];
        option->selected(true);
        refresh_visible(screen, previous_selectable_index);
    }
}

void MenuView::selection_down(MenuScreen &screen) {
    for (auto i = 0; screen.options[i] != nullptr; ++i) {
        if (screen.options[i]->selected()) {
            screen.options[i]->selected(false);
            for (auto j = i + 1; screen.options[j] != nullptr; ++j) {
                if (screen.options[j]->active()) {
                    screen.options[j]->selected(true);
                    refresh_visible(screen, j);
                    return;
                }
            }
            for (auto j = 0; screen.options[j] != nullptr; ++j) {
                if (screen.options[j]->active()) {
                    screen.options[j]->selected(true);
                    refresh_visible(screen, j);
                    return;
                }
            }
        }
    }
}

void MenuView::refresh_visible(MenuScreen &screen, int8_t selected_index) {
    static constexpr int8_t MaxiumVisible = 4;

    auto nvisible = 0u;

    for (int8_t i = 0; screen.options[i] != nullptr; ++i) {
        auto &o = screen.options[i];
        if (selected_index - i >= MaxiumVisible || nvisible >= MaxiumVisible) {
            o->visible(false);
        }
        else {
            o->visible(true);
            nvisible++;
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
    auto selectable = -1;
    for (auto i = 0; screen->options[i] != nullptr; ++i) {
        if (screen->options[i]->selected()) {
            refresh_visible(*screen, i);
            return screen;
        }
        if (screen->options[i]->active()) {
            if (selectable < 0) {
                selectable = i;
            }
        }
    }

    screen->options[selectable]->selected(true);
    refresh_visible(*screen, selectable);

    return screen;
}

}
