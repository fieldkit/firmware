#include "menu_view.h"
#include "simple_workers.h"
#include "configure_module_worker.h"
#include "factory_wipe.h"
#include "hal/board.h"

namespace fk {

template<typename T>
LambdaOption<T> *to_lambda_option(Pool &pool, const char *label, T fn) {
    return new (pool) LambdaOption<T>(label, fn);
}

template<size_t N>
MenuScreen *new_menu_screen(Pool &pool, MenuOption* (&&options)[N]) {
    auto n_options = (MenuOption **)pool.malloc(sizeof(MenuOption*) * (N + 1));
    for (size_t i = 0; i < N; ++i) {
        n_options[i] = options[i];
    }
    n_options[N] = nullptr;
    return new (pool) MenuScreen(n_options);
}

MenuView::MenuView(Pool &pool, ViewController *views) {
    auto back = to_lambda_option(pool, "Back", [=]() {
        // NOTE Fancy way of deselecting ourselves.
        // Could be selected from another back operation.
        for (auto i = 0; active_menu_->options[i] != nullptr; ++i) {
            active_menu_->options[i]->selected = false;
        }
        active_menu_ = goto_menu(previous_menu_);
        previous_menu_ = nullptr;
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
        get_display()->off();
        perform_factory_reset();
        back->on_selected();
        views->show_home();
    });
    tools_menu_ = new_menu_screen<4>(pool, {
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
    network_menu_ = new_menu_screen<2>(pool, {
        back,
        network_toggle,
    });


    auto modules_water = to_lambda_option(pool, "Water", [=]() {
        back->on_selected();
        views->show_home();
        auto worker = create_pool_wrapper<ConfigureModuleWorker, DefaultWorkerPoolSize, PoolWorker<ConfigureModuleWorker>>(ConfigureModuleKind::Water);
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    });
    auto modules_weather = to_lambda_option(pool, "Weather", [=]() {
        back->on_selected();
        views->show_home();
        auto worker = create_pool_wrapper<ConfigureModuleWorker, DefaultWorkerPoolSize, PoolWorker<ConfigureModuleWorker>>(ConfigureModuleKind::Weather);
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    });
    auto modules_ultrasonic = to_lambda_option(pool, "Ultrasonic", [=]() {
        back->on_selected();
        views->show_home();
        auto worker = create_pool_wrapper<ConfigureModuleWorker, DefaultWorkerPoolSize, PoolWorker<ConfigureModuleWorker>>(ConfigureModuleKind::Ultrasonic);
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    });
    modules_menu_ = new_menu_screen<4>(pool, {
        back,
        modules_water,
        modules_weather,
        modules_ultrasonic,
    });


    auto info_name = to_lambda_option(pool, "Name", [=]() {
        views->show_name();
        back->on_selected();
    });
    auto info_memory = to_lambda_option(pool, "Memory", [=]() {
        back->on_selected();
    });
    auto info_modules = to_lambda_option(pool, "Modules", [=]() {
        previous_menu_ = active_menu_;
        active_menu_ = goto_menu(modules_menu_);
    });
    info_menu_ = new_menu_screen<4>(pool, {
        back,
        info_name,
        info_memory,
        info_modules,
    });


    auto main_readings = to_lambda_option(pool, "Readings", [=]() {
        views->show_readings();
    });
    auto main_info = to_lambda_option(pool, "Info", [=]() {
        previous_menu_ = active_menu_;
        active_menu_ = goto_menu(info_menu_);
    });
    auto main_network = to_lambda_option(pool, "Network", [=]() {
        previous_menu_ = active_menu_;
        active_menu_ = goto_menu(network_menu_);
    });
    auto main_tools = to_lambda_option(pool, "Tools", [=]() {
        previous_menu_ = active_menu_;
        active_menu_ = goto_menu(tools_menu_);
    });

    main_menu_ = new_menu_screen<4>(pool, {
        main_readings,
        main_info,
        main_network,
        main_tools,
    });

    active_menu_ = main_menu_;
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

void MenuView::selection_down(MenuScreen &screen) {
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

MenuOption *MenuView::selected(MenuScreen &screen) {
    for (auto i = 0; screen.options[i] != nullptr; ++i) {
        if (screen.options[i]->selected) {
            return screen.options[i];
        }
    }
    FK_ASSERT(0);
    return nullptr;
}

MenuScreen *MenuView::goto_menu(MenuScreen *screen) {
    MenuOption *selectable = nullptr;
    for (auto i = 0; screen->options[i] != nullptr; ++i) {
        if (screen->options[i]->selected) {
            // loginfo("already selected %d '%s'", i, screen->options[i]->label);
            return screen;
        }
        if (selectable == nullptr) {
            selectable = screen->options[i];
        }
    }

    // loginfo("selecting %s", selectable->label);
    selectable->selected = true;

    return screen;
}

}
