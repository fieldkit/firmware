#include <samd51_common.h>

#include "menu_view.h"
#include "hal/board.h"
#include "state_ref.h"
#include "platform.h"

#include "simple_workers.h"
#include "upgrade_from_sd_worker.h"
#include "dump_flash_memory_worker.h"
#include "export_data_worker.h"
#include "compare_banks_worker.h"
#include "lora_ranging_worker.h"

#include "modules/configure_module_worker.h"
#include "modules/refresh_modules_worker.h"

#include "networking/download_firmware_worker.h"
#include "networking/upload_data_worker.h"

namespace fk {

FK_DECLARE_LOGGER("menu");

template<typename T>
LambdaOption<T> *to_lambda_option(Pool *pool, const char *label, T fn) {
    return new (*pool) LambdaOption<T>(label, fn);
}

template<size_t N>
MenuScreen *new_menu_screen(Pool *pool, const char *title, MenuOption* (&&options)[N]) {
    auto n_options = (MenuOption **)pool->malloc(sizeof(MenuOption*) * (N + 1));
    for (size_t i = 0; i < N; ++i) {
        n_options[i] = options[i];
    }
    n_options[N] = nullptr;
    return new (*pool) MenuScreen(title, n_options);
}

template<typename T>
struct NetworkOption : public MenuOption {
    int32_t index_;
    WifiNetworkInfo network_;
    T fn_;

    NetworkOption(int32_t index, T fn) : MenuOption(""), index_(index), fn_(fn) {
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

    void refresh(GlobalState const *gs) override {
        network_ = gs->network.config.wifi_networks[index_];
    }
};

template<typename T>
NetworkOption<T> *to_network_option(Pool *pool, int32_t index, T fn) {
    return new (*pool) NetworkOption<T>(index, fn);
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
        get_ipc()->launch_worker(create_pool_worker<WifiToggleWorker>());
    }

    const char *label() const override {
        if (get_network()->enabled()) {
            return "Disable";
        }
        return "Enable";
    }

    void refresh(GlobalState const *gs) override {
    }
};

void configure_gps_duration(uint32_t duration, Pool &pool) {
    auto gs = get_global_state_rw();
    gs.get()->scheduler.gps.duration = duration;
    gs.get()->flush(pool);
}

struct ToggleGpsAlwaysOnOption : public MenuOption {
    MenuOption *back_;
    ViewController *views_;
    bool always_on_{ false };

    ToggleGpsAlwaysOnOption(MenuOption *back, ViewController *views) : MenuOption(""), back_(back), views_(views) {
    }

    void on_selected() override {
        back_->on_selected();
        views_->show_home();

        StandardPool pool{ "toggle-always-on" };
        if (always_on_) {
            configure_gps_duration(TenMinutesSeconds, pool);
        }
        else {
            configure_gps_duration(UINT32_MAX, pool);
        }

        always_on_ = !always_on_;
    }

    const char *label() const override {
        if (always_on_) {
            return "Fixed Mode";
        }
        return "Moving Mode";
    }

    void refresh(GlobalState const *gs) override {
        always_on_ = gs->scheduler.gps.duration == UINT32_MAX;
    }
};

void configure_wifi_duration(uint32_t duration, Pool &pool) {
    auto gs = get_global_state_rw();
    gs.get()->scheduler.network.duration = duration;
    gs.get()->flush(pool);
    if (!get_network()->enabled()) {
        get_ipc()->launch_worker(create_pool_worker<WifiToggleWorker>());
    }
}

struct ToggleWifiAlwaysOnOption : public MenuOption {
    MenuOption *back_;
    ViewController *views_;
    bool always_on_{ false };

    ToggleWifiAlwaysOnOption(MenuOption *back, ViewController *views) : MenuOption(""), back_(back), views_(views) {
    }

    void on_selected() override {
        back_->on_selected();
        views_->show_home();

        StandardPool pool{ "toggle-always-on" };
        if (always_on_) {
            configure_wifi_duration(FiveMinutesSeconds, pool);
        }
        else {
            configure_wifi_duration(UINT32_MAX, pool);
        }

        always_on_ = !always_on_;
    }

    const char *label() const override {
        if (always_on_) {
            return "Idle Off";
        }
        return "Always On";
    }

    void refresh(GlobalState const *gs) override {
        always_on_ = gs->scheduler.network.duration == UINT32_MAX;
    }
};

MenuView::MenuView(ViewController *views, Pool &pool) : pool_(&pool), views_(views) {
    back_ = to_lambda_option(&pool, "Back", [=]() {
        back_->selected(false);

        if (previous_menu_ == nullptr || previous_menu_ == active_menu_) {
            loginfo("selected main-menu '%s'", active_menu_->title);
            active_menu_ = goto_menu(main_menu_);
        }
        else {
            loginfo("selected previous-menu '%s'", active_menu_->title);
            active_menu_ = goto_menu(previous_menu_);
        }
    });

    create_info_menu();
    create_tools_menu();
    create_module_bays_menu();
    create_module_menu();
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
    auto info_build = to_lambda_option(pool_, "Build", [=]() {
        views_->show_build();
        back_->on_selected();
    });
    auto info_qr_code = to_lambda_option(pool_, "QR Code", [=]() {
        views_->show_qr_code();
        back_->on_selected();
    });

    info_menu_ = new_menu_screen<4>(pool_, "info", {
        back_,
        info_build,
        info_name,
        info_qr_code,
    });
}

void MenuView::create_module_bays_menu() {
    auto module_bays_status = to_lambda_option(pool_, "Status", [=]() {
        back_->on_selected();
        get_ipc()->launch_worker(create_pool_worker<RefreshModulesWorker>());
        views_->show_module_status();
    });
    module_bays_menu_ = new_menu_screen<1>(pool_, "modules", {
        module_bays_status,
    });
}

void MenuView::create_module_menu() {
    auto program_weather = to_lambda_option(pool_, "Weather", [=]() {
        back_->on_selected();
        views_->show_module_status();
        ModuleHeader header = {
            .manufacturer = FK_MODULES_MANUFACTURER,
            .kind = FK_MODULES_KIND_WEATHER,
            .version = 0x01,
            .id = { 0 },
        };
        get_ipc()->launch_worker(create_pool_worker<ConfigureModuleWorker>(selected_module_bay_, header));
    });
    auto program_ph = to_lambda_option(pool_, "Water (pH)", [=]() {
        back_->on_selected();
        views_->show_module_status();
        ModuleHeader header = {
            .manufacturer = FK_MODULES_MANUFACTURER,
            .kind = FK_MODULES_KIND_WATER_PH,
            .version = 0x01,
            .id = { 0 },
        };
        get_ipc()->launch_worker(create_pool_worker<ConfigureModuleWorker>(selected_module_bay_, header));
    });
    auto program_ec = to_lambda_option(pool_, "Water (EC)", [=]() {
        back_->on_selected();
        views_->show_module_status();
        ModuleHeader header = {
            .manufacturer = FK_MODULES_MANUFACTURER,
            .kind = FK_MODULES_KIND_WATER_EC,
            .version = 0x01,
            .id = { 0 },
        };
        get_ipc()->launch_worker(create_pool_worker<ConfigureModuleWorker>(selected_module_bay_, header));
    });
    auto program_do = to_lambda_option(pool_, "Water (DO)", [=]() {
        back_->on_selected();
        views_->show_module_status();
        ModuleHeader header = {
            .manufacturer = FK_MODULES_MANUFACTURER,
            .kind = FK_MODULES_KIND_WATER_DO,
            .version = 0x01,
            .id = { 0 },
        };
        get_ipc()->launch_worker(create_pool_worker<ConfigureModuleWorker>(selected_module_bay_, header));
    });
    auto program_temp = to_lambda_option(pool_, "Water (Temp)", [=]() {
        back_->on_selected();
        views_->show_module_status();
        ModuleHeader header = {
            .manufacturer = FK_MODULES_MANUFACTURER,
            .kind = FK_MODULES_KIND_WATER_TEMP,
            .version = 0x01,
            .id = { 0 },
        };
        get_ipc()->launch_worker(create_pool_worker<ConfigureModuleWorker>(selected_module_bay_, header));
    });
    auto program_orp = to_lambda_option(pool_, "Water (ORP)", [=]() {
        back_->on_selected();
        views_->show_module_status();
        ModuleHeader header = {
            .manufacturer = FK_MODULES_MANUFACTURER,
            .kind = FK_MODULES_KIND_WATER_ORP,
            .version = 0x01,
            .id = { 0 },
        };
        get_ipc()->launch_worker(create_pool_worker<ConfigureModuleWorker>(selected_module_bay_, header));
    });
    auto program_distance = to_lambda_option(pool_, "Distance", [=]() {
        back_->on_selected();
        views_->show_module_status();
        ModuleHeader header = {
            .manufacturer = FK_MODULES_MANUFACTURER,
            .kind = FK_MODULES_KIND_DISTANCE,
            .version = 0x01,
            .id = { 0 },
        };
        get_ipc()->launch_worker(create_pool_worker<ConfigureModuleWorker>(selected_module_bay_, header));
    });
    auto program_menu = new_menu_screen<7>(pool_, "program", {
            program_weather,
            program_ph, program_ec, program_do, program_temp, program_orp,
            program_distance,
    });

    auto module_home = to_lambda_option(pool_, "Home", [=]() {
        back_->on_selected();
        views_->show_home();
    });
    auto module_program = to_lambda_option(pool_, "Program", [=]() {
        previous_menu_ = active_menu_;
        active_menu_ = goto_menu(program_menu);
    });
    auto module_erase = to_lambda_option(pool_, "Erase", [=]() {
        back_->on_selected();
        views_->show_home();
        get_ipc()->launch_worker(create_pool_worker<ConfigureModuleWorker>(selected_module_bay_));
    });

    module_menu_ = new_menu_screen<3>(pool_, "module", {
        module_home,
        module_program,
        module_erase,
    });
}

void MenuView::create_tools_menu() {
    auto tools_self_check = to_lambda_option(pool_, "Self Check", [=]() {
        views_->show_self_check();
    });
    auto tools_gps_toggle = new (*pool_) ToggleGpsAlwaysOnOption(back_, views_);
    auto tools_format_sd = to_lambda_option(pool_, "Format SD", [=]() {
        back_->on_selected();
        views_->show_message("Formatting SD");
        if (!get_sd_card()->format()) {
            views_->show_message("Card Error!");
            return;
        }
        views_->show_message("Success!");
    });
    auto tools_dump_flash = to_lambda_option(pool_, "Flash -> SD", [=]() {
        back_->on_selected();
        views_->show_home();
        get_ipc()->launch_worker(create_pool_worker<DumpFlashMemoryWorker>());
    });
    auto tools_load_firmware_sd = to_lambda_option(pool_, "SD Upgrade", [=]() {
        back_->on_selected();
        views_->show_home();
        auto params = SdCardFirmware{ SdCardFirmwareOperation::Load, "fkbl-fkb.bin", "fk-bundled-fkb.bin", true };
        get_ipc()->launch_worker(create_pool_worker<UpgradeFirmwareFromSdWorker>(params));
    });
    auto tools_fsck = to_lambda_option(pool_, "Run Fsck", [=]() {
        back_->on_selected();
        views_->show_home();
        get_ipc()->launch_worker(create_pool_worker<FsckWorker>());
    });
    auto tools_lora_ranging = to_lambda_option(pool_, "LoRa Ranging", [=]() {
        back_->on_selected();
        views_->show_lora();
        get_ipc()->launch_worker(WorkerCategory::Lora, create_pool_worker<LoraRangingWorker>(false));
    });
    auto tools_lora_ranging_confirmed = to_lambda_option(pool_, "LoRa Ranging (Cnf)", [=]() {
        back_->on_selected();
        views_->show_lora();
        get_ipc()->launch_worker(WorkerCategory::Lora, create_pool_worker<LoraRangingWorker>(true));
    });
    auto tools_gps = to_lambda_option(pool_, "GPS", [=]() {
        back_->on_selected();
        views_->show_gps();
    });
    auto tools_factory_reset = to_lambda_option(pool_, "Factory Reset", [=]() {
        back_->on_selected();
        views_->show_home();
        get_ipc()->launch_worker(create_pool_worker<FactoryWipeWorker>());
    });
    auto tools_restart = to_lambda_option(pool_, "Restart", [=]() {
        get_display()->off();
        fk_logs_flush();
        fk_restart();
    });
    auto tools_export_data = to_lambda_option(pool_, "Export CSV", [=]() {
        back_->on_selected();
        views_->show_home();
        get_ipc()->launch_worker(create_pool_worker<ExportDataWorker>());
    });

    tools_menu_ = new_menu_screen<13>(pool_, "tools", {
        back_,
        tools_self_check,
        tools_gps,
        tools_gps_toggle,
        tools_lora_ranging,
        tools_lora_ranging_confirmed,
        tools_load_firmware_sd,
        tools_dump_flash,
        tools_format_sd,
        tools_fsck,
        tools_export_data,
        tools_factory_reset,
        tools_restart,
    });
}

static WifiNetworkInfo get_self_ap_network() {
  auto gs = get_global_state_ro();
  return WifiNetworkInfo{gs.get()->general.name};
}

void MenuView::create_network_menu() {
    auto network_choose_self = to_lambda_option(pool_, "Create AP", [=]() {
        choose_active_network(get_self_ap_network());
        back_->on_selected();
        views_->show_home();
    });

    MenuOption *network_options[WifiMaximumNumberOfNetworks];
    {
        auto gs = get_global_state_ro();
        for (auto index = 0u; index < WifiMaximumNumberOfNetworks; ++index) {
            auto no = to_network_option(pool_, index, [=](WifiNetworkInfo network) {
                choose_active_network(network);
                back_->on_selected();
                views_->show_home();
            });

            network_options[index] = no;
        }
    }

    network_choose_menu_ = new_menu_screen<4>(pool_, "networks", {
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

    auto network_download_fw = to_lambda_option(pool_, "Upgrade", [=]() {
        back_->on_selected();
        views_->show_home();
        get_ipc()->launch_worker(create_pool_worker<DownloadFirmwareWorker>());
    });

    auto network_always_on_toggle = new (*pool_) ToggleWifiAlwaysOnOption(back_, views_);

    auto network_upload_resume = to_lambda_option(pool_, "Upload Rsm", [=]() {
        back_->on_selected();
        views_->show_home();
        get_ipc()->launch_worker(create_pool_worker<UploadDataWorker>(false, false));
    });

    auto network_upload_meta = to_lambda_option(pool_, "Upl All Meta", [=]() {
        back_->on_selected();
        views_->show_home();
        get_ipc()->launch_worker(create_pool_worker<UploadDataWorker>(true, false));
    });

    auto network_upload_data = to_lambda_option(pool_, "Upl All Data", [=]() {
        back_->on_selected();
        views_->show_home();
        get_ipc()->launch_worker(create_pool_worker<UploadDataWorker>(false, true));
    });

    network_menu_ = new_menu_screen<8>(pool_, "network", {
        back_,
        network_toggle,
        network_choose,
        network_upload_resume,
        network_upload_meta,
        network_upload_data,
        network_always_on_toggle,
        network_download_fw,
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

    main_menu_ = new_menu_screen<5>(pool_, "main", {
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

void MenuView::show_for_module(uint8_t bay) {
    selected_module_bay_ = bay;
    active_menu_ = goto_menu(module_menu_);
    show();
}

void MenuView::tick(ViewController *views, Pool &pool) {
    auto bus = get_board()->i2c_core();
    auto display = get_display();
    display->menu(*active_menu_);

    if (fk_uptime() > menu_time_) {
        views->show_home();
    }

    if (fk_uptime() > refresh_time_) {
        refresh();
        refresh_time_ = fk_uptime() + OneSecondMs;
    }
}

void MenuView::refresh() {
    auto gs = get_global_state_ro();
    network_choose_menu_->refresh(gs.get());
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

    loginfo("hello '%s'", network.ssid);

    gs.get()->network.config.selected = network;
    gs.get()->network.config.modified = fk_uptime();

    if (!get_network()->enabled()) {
        auto worker = create_pool_worker<WifiToggleWorker>();
        get_ipc()->launch_worker(worker);
    }
}

void MenuView::selection_up(MenuScreen &screen) {
    auto select_last = false;
    auto previous_selectable_index = -1;

    for (auto i = 0u; screen.options[i] != nullptr; ++i) {
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
    for (auto i = 0u; screen.options[i] != nullptr; ++i) {
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
    static constexpr int8_t MaximumVisible = 4;

    auto nvisible = 0u;

    for (auto i = 0; screen.options[i] != nullptr; ++i) {
        auto &o = screen.options[i];
        if (selected_index - i >= MaximumVisible || nvisible >= MaximumVisible) {
            o->visible(false);
        }
        else {
            o->visible(true);
            nvisible++;
        }
    }
}

MenuOption *MenuView::selected(MenuScreen &screen) {
    for (auto i = 0u; screen.options[i] != nullptr; ++i) {
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
