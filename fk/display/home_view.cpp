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
    screen.network.enabled = gs.get()->network.state.enabled;
    screen.network.connected = gs.get()->network.state.connected;
    screen.gps.enabled = gs.get()->gps.enabled;
    screen.gps.fix = gs.get()->gps.fix;
    screen.battery = gs.get()->power.charge;
    screen.logo = true;
    screen.progress = {
        gs.get()->progress.operation,
        gs.get()->progress.progress,
    };

    if (visible_ == 0) {
        screen.message = gs.get()->general.name;
    }
    else {
        if (gs.get()->network.state.enabled) {
            screen.message = gs.get()->network.state.ssid;
        }
        else {
            screen.message = "WiFi Off";
        }
    }

    display->home(screen);
}

void HomeView::up(ViewController *views) {
    visible_ = (visible_ - 1) % 2;
}

void HomeView::down(ViewController *views) {
    visible_ = (visible_ + 1) % 2;
}

void HomeView::enter(ViewController *views) {
    views->show_menu();
}

}
