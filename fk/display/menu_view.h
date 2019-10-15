#pragma once

#include "common.h"
#include "hal/display.h"
#include "display_views.h"
#include "state.h"

namespace fk {

class MenuView : public DisplayView {
private:
    Pool *pool_;
    ViewController *views_;
    MenuOption *back_;
    MenuScreen *active_menu_{ nullptr };
    MenuScreen *previous_menu_{ nullptr };
    MenuScreen *info_menu_;
    MenuScreen *network_menu_;
    MenuScreen *network_choose_menu_;
    MenuScreen *tools_menu_;
    MenuScreen *main_menu_;
    MenuScreen *module_bays_menu_;
    MenuScreen *module_menu_;
    uint32_t menu_time_{ 0 };
    uint8_t selected_module_bay_{ 0 };

public:
    MenuView(ViewController *views, Pool &pool);

public:
    void show() override;
    void tick(ViewController *views) override;
    void up(ViewController *views) override;
    void down(ViewController *views) override;
    void enter(ViewController *views) override;

private:
    void create_info_menu();
    void create_modules_menu();
    void create_tools_menu();
    void create_network_menu();
    void create_main_menu();

private:
    static void choose_active_network(WifiNetworkInfo network);
    static void selection_up(MenuScreen &screen);
    static void selection_down(MenuScreen &screen);
    static void refresh_visible(MenuScreen &screen, int8_t selected_index);
    static MenuOption *selected(MenuScreen &screen);
    static MenuScreen *goto_menu(MenuScreen *screen);

};

}
