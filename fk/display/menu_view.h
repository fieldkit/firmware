#pragma once

#include "common.h"
#include "display_views.h"
#include "hal/display.h"
#include "state.h"

namespace fk {

class GotoMenu {
public:
    virtual MenuScreen *goto_menu(MenuScreen *screen, uint32_t hold_time = FiveSecondsMs,
                                  MenuScreen *previous_menu = nullptr) = 0;
};

class MenuView : public DisplayView, GotoMenu {
private:
    Pool *pool_{ nullptr };
    ViewController *views_{ nullptr };
    MenuOption *back_{ nullptr };
    MenuScreen *active_menu_{ nullptr };
    MenuScreen *previous_menu_{ nullptr };
    MenuScreen *info_menu_{ nullptr };
    MenuScreen *network_menu_{ nullptr };
    MenuScreen *network_choose_menu_{ nullptr };
    MenuScreen *tools_menu_{ nullptr };
    MenuScreen *toggle_gps_menu_{ nullptr };
    MenuScreen *toggle_wifi_menu_{ nullptr };
    MenuScreen *main_menu_{ nullptr };
    MenuScreen *module_bays_menu_{ nullptr };
    MenuScreen *module_menu_{ nullptr };
    MenuScreen *schedules_menu_{ nullptr };
    MenuScreen *confirm_menu_{ nullptr };
    uint32_t menu_time_{ 0 };
    uint32_t hold_time_{ FiveSecondsMs };
    uint32_t refresh_time_{ 0 };
    ModulePosition selected_module_bay_{ 0 };
    MenuOption *pending_{ nullptr };
    MenuScreen *readings_menu_{ nullptr };

public:
    MenuView(ViewController *views, Pool &pool);

public:
    void show() override;
    void show_for_module(uint8_t bay);
    void tick(ViewController *views, Pool &pool) override;
    void up(ViewController *views) override;
    void down(ViewController *views) override;
    void enter(ViewController *views) override;
    void confirm(MenuOption *pending);

private:
    void create_info_menu();
    void create_module_bays_menu();
    void create_module_menu();
    void create_tools_menu();
    void create_network_menu();
    void create_main_menu();
    void create_schedules_menu();
    void create_confirmation_menu();
    void refresh();
    MenuScreen *goto_menu(MenuScreen *screen, MenuScreen *previous_menu) {
        return goto_menu(screen, FiveSecondsMs, previous_menu);
    }
    MenuScreen *goto_menu(MenuScreen *screen, uint32_t hold_time = FiveSecondsMs,
                          MenuScreen *previous_menu = nullptr) override;

private:
    static void choose_active_network(WifiNetworkInfo network);
    static void focus_up(MenuScreen &screen);
    static void focus_down(MenuScreen &screen);
    static void refresh_visible(MenuScreen &screen, int8_t focused_index);
    static MenuOption *selected(MenuScreen &screen);
};

} // namespace fk
