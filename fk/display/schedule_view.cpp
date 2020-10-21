#include <tiny_printf.h>

#include "schedule_view.h"
#include "state_manager.h"
#include "state_ref.h"
#include "hal/board.h"
#include "hal/display.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("schedview");

struct Option {
    const char *label;
    int32_t interval;
};

static constexpr size_t NumberOfOptions = 7;

static Option options[NumberOfOptions] = {
    { "Set 1min", 60 },
    { "Set 5min", 60 * 5 },
    { "Set 10min", 60 * 10 },
    { "Set 30min", 60 * 30 },
    { "Set 60min", 60 * 60 },
    { "Cancel", -1 },
    { "Never (!)", 0 }
};

void ScheduleView::tick(ViewController *views, Pool &pool) {
    auto bus = get_board()->i2c_core();
    auto display = get_display();

    auto now = fk_uptime();
    if (update_at == 0 || now > update_at) {
        scheduled_ = fk_schedule_get_scheduled_time();
        update_at = now + FiveSecondsMs;
    }

    char secondary[64] = { 0 };
    tiny_snprintf(secondary, sizeof(secondary), "Next: %" PRIu32 "s", scheduled_.seconds);

    auto option = options[position_ % NumberOfOptions];
    display->simple(SimpleScreen{ option.label, secondary });
}

void ScheduleView::up(ViewController *views) {
    position_--;
}

void ScheduleView::down(ViewController *views) {
    position_++;
}

void ScheduleView::enter(ViewController *views) {
    auto& option = options[position_ % NumberOfOptions];
    if (option.interval >= 0) {
        GlobalStateManager gsm;
        gsm.apply([=](GlobalState *gs) {
            StandardPool pool{ "flush" };
            loginfo("selected: %s", option.label);
            gs->scheduler.readings.simple(option.interval);
            gs->flush(pool);
        });

        // Save
    }

    views->show_home();
}

}
