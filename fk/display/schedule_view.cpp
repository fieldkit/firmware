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
        auto gs = get_global_state_ro();
        scheduled_ = gs.get()->scheduler.upcoming;
        interval_ = gs.get()->scheduler.readings.interval;
        update_at_ = now + OneSecondMs;
    }

    char primary[64] = { 0 };
    char secondary[64] = { 0 };

    auto option = options[position_ % NumberOfOptions];
    auto selected = option.interval == interval_ ? "*" : "";

    if ((int32_t)scheduled_.seconds > option.interval) {
        logwarn("unusual schedule: interval: %" PRId32 " remaining: %" PRIu32 " now: %" PRIu32 " schival: %" PRIu32,
                option.interval, scheduled_.seconds, now, interval_);
    }

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
        GlobalStateManager gsm;
        gsm.apply([=](GlobalState *gs) {
            StandardPool pool{ "flush" };
            loginfo("selected: %s", option.label);
            gs->scheduler.readings.simple(option.interval);
            gs->flush(pool);
            update_at_ = 0; // Force display refresh.
        });
    }

    views->show_home();
}

} // namespace fk
