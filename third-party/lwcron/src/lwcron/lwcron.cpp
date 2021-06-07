#include "lwcron.h"
#include <stdio.h>

namespace lwcron {

constexpr uint32_t SecondsPerYear = (60 * 60 * 24L * 365);
constexpr uint32_t SecondsPerDay = 60 * 60 * 24L;
constexpr uint32_t SecondsPerHour = 3600L;
constexpr uint32_t RerunThreshold = 30;

constexpr uint8_t DaysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

constexpr bool is_leap_year(uint16_t year) {
    return year % 4 == 0;
}

DateTime::DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) :
    year_(year), month_(month - 1), day_(day), hour_(hour), minute_(minute), second_(second) {
}

DateTime::DateTime(uint32_t unix_time) {
    auto t = unix_time;

    TimeOfDay tod{ t };
    second_ = tod.second;
    minute_ = tod.minute;
    hour_ = tod.hour;
    t = tod.remainder;

    auto year = 70;
    auto days = 0;
    while ((uint16_t)(days += (is_leap_year(year) ? 366 : 365)) <= t) {
        year++;
    }

    year_ = year + 1900;
    days -= is_leap_year(year) ? 366 : 365;
    t -= days;

    auto month = 0;
    auto length = 0;
    for (month = 0; month < 12; month++) {
        if (month == 1) { // february
            if (is_leap_year(year)) {
                length = 29;
            } else {
                length = 28;
            }
        } else {
            length = DaysInMonth[month];
        }

        if (t >= length) {
            t -= length;
        }
        else {
            break;
        }
    }

    month_ = month;
    day_ = t + 1;
}

uint32_t DateTime::unix_time() {
    auto year = year_;
    auto seconds = (year - 1970) * SecondsPerYear;
    for (auto i = 1970; i < year; i++) {
        if (is_leap_year(i)) {
            seconds += SecondsPerDay;
        }
    }
    for (auto i = 0; i < month_; i++) {
        if (i == 1 && is_leap_year(year)) {
            seconds += SecondsPerDay * 29;
        }
        else {
            seconds += SecondsPerDay * DaysInMonth[i];
        }
    }

    seconds += (day_ - 1) * SecondsPerDay;
    seconds += hour_ * SecondsPerHour;
    seconds += minute_ * 60L;
    seconds += second_;
    return seconds;
}

void PeriodicTask::run() {
}

bool PeriodicTask::valid() const {
    return interval_ > 0;
}

bool PeriodicTask::enabled() const {
    return true;
}

uint32_t PeriodicTask::getNextTime(DateTime after, uint32_t seed) const {
    auto seconds = after.unix_time();
    auto r = seconds % interval_;
    if (r == 0) {
        return seconds;
    }
    return seconds + (interval_ - r);
}

CronSpec CronSpec::interval(uint32_t seconds) {
    CronSpec cs;

    if (seconds > 0) {
        for (auto s = 0u; s <= SecondsPerDay; s += seconds) {
            TimeOfDay tod{ s };
            cs.set(tod);
        }
    }

    return cs;
}

CronSpec CronSpec::specific(uint8_t second, uint8_t minute, uint8_t hour) {
    CronSpec cs;
    bitarray_set(cs.seconds, second);
    if (minute == 0xff) {
        memset(cs.minutes, 0xff, sizeof(cs.minutes));
    }
    else {
        bitarray_set(cs.minutes, minute);
    }
    if (hour == 0xff) {
        memset(cs.hours, 0xff, sizeof(cs.hours));
    }
    else {
        bitarray_set(cs.hours, hour);
    }
    return cs;
}


CronSpec CronSpec::everyFiveMinutes() {
    CronSpec cs;
    bitarray_set(cs.seconds, 0);
    for (auto i = 0; i < 60; i += 5) {
        bitarray_set(cs.minutes, i);
    }
    memset(cs.hours, 0xff, sizeof(cs.hours));
    return cs;
}

CronSpec CronSpec::everyTwentyMinutes() {
    CronSpec cs;
    bitarray_set(cs.seconds, 0);
    for (auto i = 0; i < 60; i += 20) {
        bitarray_set(cs.minutes, i);
    }
    memset(cs.hours, 0xff, sizeof(cs.hours));
    return cs;
}

void CronSpec::clear() {
    bzero(&hours, sizeof(hours));
    bzero(&minutes, sizeof(minutes));
    bzero(&seconds, sizeof(seconds));
}

void CronSpec::set(TimeOfDay tod) {
    bitarray_set(hours, tod.hour);
    bitarray_set(minutes, tod.minute);
    bitarray_set(seconds, tod.second);
}

bool CronSpec::valid() const {
    return bitarray_any(seconds) && bitarray_any(minutes) && bitarray_any(hours);
}

bool CronTask::enabled() const {
    return true;
}

// NOTE: This could be so much better.
uint32_t CronSpec::getNextTime(DateTime after) const {
    if (!valid()) {
        return 0;
    }

    auto unix_time = after.unix_time();
    DateTime date_time{ unix_time };
    auto hour = date_time.hour();
    auto minute = date_time.minute();
    auto second = date_time.second();
    auto seconds = 0u;

    while (seconds < 86400) {
        CronSpec cs{ hour, minute, second };
        cs.set(TimeOfDay{ hour, minute, second });

        if (matches(cs)) {
            return unix_time + seconds;
        }

        if (matches_seconds(cs)) {
            if (matches_minutes(cs)) {
                hour++;
                seconds += 3600;
            }
            else {
                minute++;
                seconds += 60;
            }
        }
        else {
            second++;
            seconds++;
        }

        if (second == 60) {
            second = 0;
            minute++;
        }

        if (minute == 60) {
            minute = 0;
            hour++;
        }

        if (hour == 24) {
            hour = 0;
        }
    }

    return 0;
}

void CronTask::run() {
}

bool CronTask::valid() const {
    return spec_.valid();
}

uint32_t CronTask::getNextTime(DateTime after, uint32_t seed) const {
    auto unjittered = spec_.getNextTime(after);
    if (jitter_ == 0 || seed == 0) {
        return unjittered;
    }
    return unjittered + (seed % jitter_);
}

void Scheduler::begin(DateTime now) {
    for (auto i = (size_t)0; i < size_; i++) {
        auto task = tasks_[i];
        if (task->valid() && task->enabled()) {
            task->scheduled_ = task->getNextTime(now, 0);
        }
    }
}

Scheduler::TaskAndTime Scheduler::check(DateTime now, uint32_t seed) {
    auto now_unix = now.unix_time();
    auto difference = (int64_t)now_unix - (int64_t)last_now_;

    last_now_ = now_unix;

    if (difference < 0) {
        if (-difference > RerunThreshold) {
            begin(now);
            return { };
        }
    }

    for (auto i = (size_t)0; i < size_; i++) {
        auto task = tasks_[i];
        if (task->valid() && task->enabled()) {
            if (task->scheduled_ <= now_unix) {
                auto scheduled = task->scheduled_;
                task->scheduled_ = task->getNextTime(now + 1, seed);
                task->run();
                return TaskAndTime { scheduled, task };
            }
        }
    }

    return { };
}

Scheduler::TaskAndTime Scheduler::nextTask(DateTime now, uint32_t seed) {
    TaskAndTime found;
    for (auto i = (size_t)0; i < size_; i++) {
        auto task = tasks_[i];
        if (task->valid() && task->enabled()) {
            auto time = task->getNextTime(now, seed);
            if (!found || found.time > time) {
                found = TaskAndTime { time, task };
            }
        }
    }
    return found;
}

Scheduler::TaskAndTime Scheduler::nextTask() {
    TaskAndTime found;
    for (auto i = (size_t)0; i < size_; i++) {
        auto task = tasks_[i];
        if (task->valid() && task->enabled()) {
            auto time = task->scheduled_;
            if (!found || found.time > time) {
                found = TaskAndTime { time, task };
            }
        }
    }
    return found;
}

}
