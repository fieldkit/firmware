#include <loading.h>
#include <tiny_printf.h>

#include "home_view.h"
#include "hal/board.h"
#include "hal/display.h"
#include "hal/network.h"
#include "state_ref.h"
#include "platform.h"
#include "utilities.h"
#include "clock.h"

#if defined(__SAMD51__)
#include "hal/metal/metal_ipc.h"
#else
#include "hal/linux/linux_ipc.h"
#endif

extern const struct fkb_header_t fkb_header;

namespace fk {

ViewController *ViewController::instance_{ nullptr };

void HomeView::tick(ViewController *views, Pool &pool) {
    auto bus = get_board()->i2c_core();
    auto display = get_display();
    auto gs = get_global_state_ro();
    auto debug_mode = fk_debug_mode();
    auto workers = get_ipc()->get_workers_display_info(pool);

    HomeScreen screen;
    screen.time = fk_uptime();
    screen.recording = gs.get()->general.recording;
    screen.network.enabled = gs.get()->network.state.enabled;
    screen.network.connected = gs.get()->network.state.connected;
    screen.network.bytes_rx = gs.get()->network.state.bytes_rx;
    screen.network.bytes_tx = gs.get()->network.state.bytes_tx;
    screen.readings = gs.get()->readings.nreadings;
    screen.gps.enabled = gs.get()->gps.enabled;
    screen.gps.fix = gs.get()->gps.fix;
    screen.power = {
        .battery = gs.get()->power.charge,
    };
    screen.debug_mode = debug_mode;
    screen.logo = true;
    screen.primary = nullptr;
    screen.secondary = nullptr;
    screen.progress = {
        gs.get()->progress.operation,
        gs.get()->progress.progress,
    };

    switch ((Visible)visible_) {
    case Visible::Name: {
        auto name = gs.get()->general.name;
        auto first_space = strchr(name, ' ');
        if (first_space != nullptr) {
            auto pl = (size_t)(first_space - name);
            if (pl < sizeof(primary_)) {
                memcpy(primary_, name, pl);
                primary_[pl] = 0;
                screen.primary = primary_;
            }

            auto sl = strlen(first_space + 1);
            if (sl < sizeof(secondary_)) {
                memcpy(secondary_, first_space + 1, sl);
                secondary_[sl] = 0;
                screen.secondary = secondary_;
            }
        }
        else {
            screen.primary = name;
        }
        break;
    }
    case Visible::IP: {
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
    case Visible::Build: {
        if ((screen.time / 5000) % 2 == 0) {
            DateTime ts{ fkb_header.firmware.timestamp };
            tiny_snprintf(primary_, sizeof(primary_), "Build #%" PRIu32, fkb_header.firmware.number);
            tiny_snprintf(secondary_, sizeof(secondary_), "%02d%02d-%02d%02d", ts.month(), ts.day(), ts.hour(), ts.minute());
        }
        else {
            auto hash_dash = strrchr((char const *)fkb_header.firmware.version, '-');
            if (hash_dash != nullptr) {
                auto prefix_length = hash_dash - (char const *)fkb_header.firmware.version;
                auto copying = std::min<size_t>(prefix_length, sizeof(primary_));
                strncpy(primary_, (char const *)fkb_header.firmware.version, copying);
                strncpy(secondary_, hash_dash + 1, sizeof(secondary_));
            }
        }
        screen.primary = primary_;
        screen.secondary = secondary_;
        break;
    }
    case Visible::Uptime: {
        make_pretty_time_string(fk_uptime(), primary_, sizeof(primary_));
        if (gs.get()->storage.is_phylum()) {
            strncpy(secondary_, "phylum-fs", sizeof(secondary_));
        }
        else {
            strncpy(secondary_, "legacy-fs", sizeof(secondary_));
        }
        screen.primary = primary_;
        screen.secondary = secondary_;
        break;
    }
    }

    for (auto i = 0u; i < NumberOfWorkerTasks; ++i) {
        screen.workers[i].visible = false;
    }

    auto index = 0u;
    for (auto &info : workers) {
        screen.workers[index].visible = info.visible;
        screen.workers[index].name = info.name;
        screen.workers[index].progress = info.progress;
        index++;
    }

    display->home(screen);
}

void HomeView::up(ViewController *views) {
    visible_ = (visible_ + 4 - 1) % 4;
}

void HomeView::down(ViewController *views) {
    visible_ = (visible_ + 1) % 4;
}

void HomeView::enter(ViewController *views) {
    views->show_menu();
}

void HomeView::show_name() {
    visible_ = 0;
}

void HomeView::show_build() {
    visible_ = 2;
}

void HomeView::show_uptime() {
    visible_ = 3;
}

}
