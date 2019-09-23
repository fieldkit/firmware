#pragma once

#include "common.h"
#include "hal/display.h"
#include "display_views.h"

namespace fk {

class MenuView : public DisplayView {
private:
    MenuScreen *active_menu_{ nullptr };
    MenuScreen *previous_menu_{ nullptr };
    MenuScreen *info_menu_;
    MenuScreen *network_menu_;
    MenuScreen *tools_menu_;
    MenuScreen *main_menu_;
    MenuScreen *modules_menu_;
    uint32_t menu_time_{ 0 };

public:
    MenuView(Pool &pool, ViewController *views);

public:
    void show() override;
    void tick(ViewController *views) override;
    void up(ViewController *views) override;
    void down(ViewController *views) override;
    void enter(ViewController *views) override;

private:
    static void perform_factory_reset();
    static void selection_up(MenuScreen &screen);
    static void selection_down(MenuScreen &screen);
    static MenuOption *selected(MenuScreen &screen);
    static MenuScreen *goto_menu(MenuScreen *screen);

};

}
