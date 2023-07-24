#if defined(__SAMD51__)

#include <cstdlib>
#include <compiler.h>
#include <hal_init.h>
#include <hal/include/hpl_calendar.h>
#include <hal/include/hal_calendar.h>
#include "hal/clock.h"
#include "hal/metal/pcf2127.h"
#include "hal/metal/pcf85363a.h"
#include "config.h"

namespace fk {

FK_DECLARE_LOGGER("clock");

#if defined(FK_UNDERWATER)
static Pcf2127 external_clock;
#else
static Pcf85363a external_clock;
#endif

struct calendar_descriptor CALENDAR_0;

static CoreClock clock;

CoreClock *get_clock() {
    return &clock;
}

uint32_t get_clock_now() {
    return get_clock()->now().unix_time();
}

static void CALENDAR_0_initialize(void) {
    hri_mclk_set_APBAMASK_RTC_bit(MCLK);
    calendar_init(&CALENDAR_0, RTC);
}

CoreClock::CoreClock() {
}

bool CoreClock::begin() {
    CALENDAR_0_initialize();
    calendar_enable(&CALENDAR_0);

    if (!configure()) {
        return false;
    }

    if (!sync()) {
        return false;
    }

    return true;
}

bool CoreClock::configure() {
    if (configured_) {
        return true;
    }

    if (!external_clock.configure()) {
        return false;
    }

    configured_ = true;

    return true;
}

bool CoreClock::sync() {
    DateTime internal_time;
    if (!internal(internal_time)) {
        return false;
    }

    DateTime external_time;
    if (!external(external_time)) {
        return false;
    }

    // Set the internal time from our trusted, external source.
    struct calendar_time time;
    time.hour = external_time.hour();
    time.min = external_time.minute();
    time.sec = external_time.second();
    calendar_set_time(&CALENDAR_0, &time);

    struct calendar_date date;
    date.year = external_time.year();
    date.month = external_time.month();
    date.day = external_time.day();
    calendar_set_date(&CALENDAR_0, &date);

    FormattedTime external_formatted{ external_time.unix_time() };
    loginfo("external %s (%" PRIu32 ")", external_formatted.cstr(), external_time.unix_time());

    if (!internal(internal_time)) {
        return false;
    }

    FormattedTime internal_formatted{ internal_time.unix_time() };
    loginfo("internal %s (%" PRIu32 ")", internal_formatted.cstr(), internal_time.unix_time());

    return true;
}

bool CoreClock::adjust_internal(DateTime now) {
    struct calendar_time time;
    time.hour = now.hour();
    time.min = now.minute();
    time.sec = now.second();
    calendar_set_time(&CALENDAR_0, &time);

    struct calendar_date date;
    date.year = now.year();
    date.month = now.month();
    date.day = now.day();
    calendar_set_date(&CALENDAR_0, &date);

    return true;
}

bool CoreClock::adjust(DateTime now) {
    if (!external_clock.adjust(now)) {
        return false;
    }

    return adjust_internal(now);
}

bool CoreClock::internal(DateTime &time) {
    struct calendar_date_time mcu_time;
    calendar_get_date_time(&CALENDAR_0, &mcu_time);

    time = DateTime{
        mcu_time.date.year, mcu_time.date.month, mcu_time.date.day, mcu_time.time.hour, mcu_time.time.min, mcu_time.time.sec,
    };

    return true;
}

bool CoreClock::external(DateTime &time) {
    return external_clock.read(time);
}

DateTime CoreClock::now() {
    DateTime time;
    if (!internal(time)) {
        logerror("error getting internal time");
    }
    return time;
}

DateTime CoreClock::get_external() {
    DateTime time;
    if (!external(time)) {
        logerror("error getting external time");
        return DateTime{};
    }
    else {
        loginfo("have external time");
    }

    return time;
}

void CoreClock::compare() {
    DateTime internal_time;
    if (!internal(internal_time)) {
        return;
    }

    DateTime external_time;
    if (!external(external_time)) {
        return;
    }

    auto internal_unix = internal_time.unix_time();
    auto external_unix = external_time.unix_time();
    auto diff = (int64_t)external_unix - internal_unix;

    if (diff > TimeDriftWarnThreshold || diff < -TimeDriftWarnThreshold) {
        FormattedTime internal_formatted{ internal_unix };
        FormattedTime external_formatted{ external_unix };
        loginfo("drift: '%s' -> '%s' (%" PRIu32 " - %" PRIu32 " = %" PRId64 ")", external_formatted.cstr(), internal_formatted.cstr(),
                external_unix, internal_unix, diff);
        if (!adjust_internal(external_time)) {
            logerror("error adjusting internal clock");
        }
    }
}

uint32_t clock_adjust(uint32_t new_epoch) {
    auto clock = get_clock();
    auto previous_external = clock->get_external();
    if (previous_external.year() == 0) {
        return 0;
    }

    auto previous_epoch = previous_external.unix_time();

    clock->adjust(new_epoch);

    FormattedTime new_formatted{ new_epoch };
    FormattedTime old_formatted{ previous_epoch };
    loginfo("utc: '%s' -> '%s' (%" PRIu32 " - %" PRIu32 " = %" PRId64 ")", old_formatted.cstr(), new_formatted.cstr(), previous_epoch,
            new_epoch, (int64_t)new_epoch - previous_epoch);

    return new_epoch;
}

} // namespace fk

#endif