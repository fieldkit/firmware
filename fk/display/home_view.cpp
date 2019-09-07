#include "home_view.h"
#include "hal/board.h"
#include "hal/display.h"
#include "state_ref.h"
#include "platform.h"

namespace fk {

void HomeView::tick(ViewController *views) {
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

void HomeView::up(ViewController *views) {
    views->show_menu();
}

void HomeView::down(ViewController *views) {
    views->show_menu();
}

void HomeView::enter(ViewController *views) {
    views->show_menu();
}

}
