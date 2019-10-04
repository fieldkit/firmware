#include <loading.h>
#include <tiny_printf.h>

#include "home_view.h"
#include "hal/board.h"
#include "hal/display.h"
#include "hal/network.h"
#include "state_ref.h"
#include "platform.h"

extern const struct fkb_header_t fkb_header;

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
    screen.network.bytes_rx = gs.get()->network.state.bytes_rx;
    screen.network.bytes_tx = gs.get()->network.state.bytes_tx;
    screen.gps.enabled = gs.get()->gps.enabled;
    screen.gps.fix = gs.get()->gps.fix;
    screen.battery = gs.get()->power.charge;
    screen.logo = true;
    screen.primary = nullptr;
    screen.secondary = nullptr;
    screen.progress = {
        gs.get()->progress.operation,
        gs.get()->progress.progress,
    };

    switch (visible_) {
    case 0: {
        screen.primary = gs.get()->general.name;
        break;
    }
    case 1: {
        if (gs.get()->network.state.enabled) {
            screen.primary = gs.get()->network.state.ssid;
            ip4_address ip{ gs.get()->network.state.ip };
            tiny_snprintf(secondary_, sizeof(secondary_), "%d.%d.%d.%d", ip.u.bytes[0], ip.u.bytes[1], ip.u.bytes[2], ip.u.bytes[3]);
            screen.secondary = secondary_;
        }
        else {
            screen.primary = "WiFi Off";
        }
        break;
    }
    case 2: {
        tiny_snprintf(primary_, sizeof(primary_), "Build #%" PRIu32, fkb_header.firmware.number);
        screen.primary = primary_;
        break;
    }
    }


    display->home(screen);
}

void HomeView::up(ViewController *views) {
    visible_ = (visible_ - 1) % 3;
    alogf(LogLevels::INFO, "menu", "screen: %d", visible_);
}

void HomeView::down(ViewController *views) {
    visible_ = (visible_ + 1) % 3;
    alogf(LogLevels::INFO, "menu", "screen: %d", visible_);
}

void HomeView::enter(ViewController *views) {
    views->show_menu();
}

}
