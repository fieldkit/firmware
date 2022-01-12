#include <tiny_printf.h>

#include "hal/board.h"
#include "hal/display.h"
#include "platform.h"
#include "schedule_view.h"
#include "state_manager.h"
#include "state_ref.h"

namespace fk {

FK_DECLARE_LOGGER("schedview");

struct Option {
    const char *label;
    int32_t interval;
};

static constexpr size_t NumberOfOptions = 7;

static Option options[NumberOfOptions] = { { "Set 1min", 60 },       { "Set 5min", 60 * 5 },   { "Set 10min", 60 * 10 },
                                           { "Set 30min", 60 * 30 }, { "Set 60min", 60 * 60 }, { "Cancel", -1 },
                                           { "Never (!)", 0 } };

void ScheduleView::tick(ViewController *views, Pool &pool) {
    auto bus = get_board()->i2c_core();
    auto display = get_display();

    auto now = fk_uptime();
    if (update_at_ == 0 || now > update_at_) {
        auto schedule = get();
        scheduled_ = schedule.upcoming;
        interval_ = schedule.interval;
        update_at_ = now + OneSecondMs;
    }

    char primary[64] = { 0 };
    char secondary[64] = { 0 };

    auto option = options[position_ % NumberOfOptions];
    auto selected = option.interval == interval_ ? "*" : "";

    tiny_snprintf(primary, sizeof(primary), "%s%s", option.label, selected);
    tiny_snprintf(secondary, sizeof(secondary), "Next: %" PRIu32 "s", scheduled_.seconds);

    display->simple(SimpleScreen{ primary, secondary });
}

void ScheduleView::up(ViewController *views) {
    position_--;
}

void ScheduleView::down(ViewController *views) {
    position_++;
}

void ScheduleView::enter(ViewController *views) {
    auto &option = options[position_ % NumberOfOptions];
    if (option.interval >= 0) {
        loginfo("selected: %s", option.label);
        set(option.interval);
    }

    views->show_home();
}

Schedule ScheduleView::get() {
    auto gs = get_global_state_ro();
    switch (type_) {
    case ScheduleType::Readings: {
        return gs.get()->scheduler.readings;
    }
    case ScheduleType::LoRa: {
        return gs.get()->scheduler.lora;
    }
    default: {
        return gs.get()->scheduler.readings;
    }
    }
}

void ScheduleView::type(ScheduleType type) {
    type_ = type;
    update_at_ = 0; // Force display refresh.
}

void ScheduleView::set(uint32_t interval) {
    GlobalStateManager gsm;
    gsm.apply([=](GlobalState *gs) {
        switch (type_) {
        case ScheduleType::Readings: {
            gs->scheduler.readings.simple(interval);
            break;
        }
        case ScheduleType::LoRa: {
            gs->scheduler.lora.simple(interval);
            break;
        }
        }

        StandardPool pool{ "flush" };
        gs->flush(pool);
    });

    update_at_ = 0; // Force display refresh.
}

} // namespace fk
