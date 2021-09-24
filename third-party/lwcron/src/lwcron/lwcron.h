#ifndef LWCRON_H_INCLUDED
#define LWCRON_H_INCLUDED

#include <cinttypes>
#include <cstring>
#include <cstdio>

namespace lwcron {

struct TimeOfDay {
    int32_t hour;
    int32_t minute;
    int32_t second;
    int32_t remainder;

    TimeOfDay(int32_t hour, int32_t minute, int32_t second) : hour(hour), minute(minute), second(second), remainder(0) {
    }

    TimeOfDay(uint32_t t) {
        second = t % 60;
        t /= 60;
        minute = t % 60;
        t /= 60;
        hour = t % 24;
        t /= 24;
        remainder = t;
    }
};

class DateTime {
private:
    uint16_t year_{ 0 };
    uint8_t month_{ 0 }, day_{ 0 }, hour_{ 0 }, minute_{ 0 }, second_{ 0 };

public:
    DateTime() {
    }

    DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
    DateTime(uint32_t time);

public:
    uint16_t year() const {
        return year_;
    }
    uint8_t month() const {
        return month_ + 1;
    }
    uint8_t day() const {
        return day_;
    }
    uint8_t hour() const {
        return hour_;
    }
    uint8_t minute() const {
        return minute_;
    }
    uint8_t second() const {
        return second_;
    }

public:
    uint32_t unix_time();

public:
    DateTime operator+(const uint32_t seconds) {
        return DateTime(unix_time() + seconds);
    }

    DateTime operator-(const uint32_t seconds) {
        return DateTime(unix_time() - seconds);
    }

    DateTime& operator+=(const uint32_t rhs){
        *this = DateTime(unix_time() + rhs);
        return *this;
    }

    DateTime& operator-=(const uint32_t rhs){
        *this = DateTime(unix_time() - rhs);
        return *this;
    }

    bool operator ==(const DateTime &b) const {
        return year_ == b.year_ && month_ == b.month_ &&
               day_ == b.day_ && hour_ == b.hour_ &&
               minute_ == b.minute_ && second_ == b.second_;
    }

    bool operator !=(const DateTime &b) const {
        return !(*this == b);
    }

};

class Scheduler;
class PeriodicTask;
class CronTask;

class TaskVisitor {
public:
    virtual void visit(PeriodicTask &task);
    virtual void visit(CronTask &task);

};

class Task {
private:
    uint32_t scheduled_{ 0 };
    bool pending_{ false };

public:
    virtual void run() = 0;
    virtual bool valid() const = 0;
    virtual bool enabled() const = 0;
    virtual uint32_t getNextTime(DateTime after, uint32_t seed) const = 0;
    virtual void accept(TaskVisitor &visitor) = 0;
    virtual const char *toString() const {
        return "Task<>";
    }

public:
    friend class Scheduler;

};

class PeriodicTask : public Task {
private:
    uint32_t interval_{ 0 };

public:
    PeriodicTask() {
    }

    PeriodicTask(uint32_t interval) : interval_(interval) {
    }

public:
    uint32_t interval() const {
        return interval_;
    }

public:
    void run() override;
    bool valid() const override;
    bool enabled() const override;
    uint32_t getNextTime(DateTime after, uint32_t seed) const override;
    void accept(TaskVisitor &visitor) override {
        visitor.visit(*this);
    }

};

template<size_t N>
bool bitarray_any(const uint8_t (&bytes)[N]) {
    for (auto i = (size_t)0; i < N; ++i) {
        if (bytes[i] > 0) {
            return true;
        }
    }
    return false;
}

template<size_t N>
static inline bool bitarray_test(uint8_t (&p)[N], uint32_t n) {
    return p[n / 8] & (0x1 << (n % 8));
}

template<size_t N>
uint32_t bitarray_nset(uint8_t (&bytes)[N]) {
    auto c = 0u;
    for (auto i = 0u; i < N * 8u; ++i) {
        if (bitarray_test(bytes, i)) {
            c++;
        }
    }
    return c;
}

template<size_t N>
static inline void bitarray_clear_set(uint8_t (&p)[N], uint32_t n) {
    bzero(&p, sizeof(p));
    p[n / 8] |= (0x1 << (n % 8));
}

template<size_t N>
static inline void bitarray_set(uint8_t (&p)[N], uint32_t n) {
    p[n / 8] |= (0x1 << (n % 8));
}

template<size_t N>
static inline void bitarray_clear(uint8_t (&p)[N], uint32_t n) {
    p[n / 8] &= ~(0x1 << (n % 8));
}

struct CronSpec {
public:
    uint8_t seconds[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t minutes[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t hours[3] = { 0, 0, 0 };

public:
    CronSpec() {
    }

    CronSpec(TimeOfDay tod) {
        set(tod);
    }

    CronSpec(int32_t hours, int32_t minutes, int32_t seconds) {
        bitarray_set(this->hours, hours);
        bitarray_set(this->minutes, minutes);
        bitarray_set(this->seconds, seconds);
    }

    CronSpec(DateTime const &when) {
        bitarray_set(seconds, when.second());
        bitarray_set(minutes, when.minute());
        bitarray_set(hours, when.hour());
    }

public:
    bool valid() const;

    void clear();

    void set(TimeOfDay tod);

    uint32_t getNextTime(DateTime after) const;

    static CronSpec interval(uint32_t seconds);

    static CronSpec specific(uint8_t second, uint8_t minute = 0xff, uint8_t hour = 0xff);

    static CronSpec everyFiveMinutes();

    static CronSpec everyTwentyMinutes();

    inline bool operator==(CronSpec const &rhs) const {
        return memcmp(hours, rhs.hours, sizeof(hours)) == 0 &&
               memcmp(minutes, rhs.minutes, sizeof(minutes)) == 0 &&
               memcmp(seconds, rhs.seconds, sizeof(seconds)) == 0;
    }

    inline bool operator!=(CronSpec const &rhs) const {
        return !(*this == rhs);
    }

private:
    bool matches(CronSpec const &cs) const {
        return matches_hours(cs) && matches_minutes(cs) && matches_seconds(cs);
    }

    bool matches_hours(CronSpec const &cs) const {
        return matches(hours, cs.hours, sizeof(hours));
    }

    bool matches_minutes(CronSpec const &cs) const {
        return matches(minutes, cs.minutes, sizeof(minutes));
    }

    bool matches_seconds(CronSpec const &cs) const {
        return matches(seconds, cs.seconds, sizeof(seconds));
    }

    static bool matches(uint8_t const *a, uint8_t const *b, size_t size) {
        for (auto i = 0u; i < size; ++i) {
            if ((a[i] & b[i]) > 0) {
                return true;
            }
        }
        return false;
    }
};

class CronTask : public Task {
private:
    CronSpec spec_;
    uint32_t jitter_;

public:
    CronTask() : jitter_(0) {
    }

    CronTask(CronSpec spec) : spec_(spec), jitter_(0) {
    }

    CronTask(CronSpec spec, uint32_t jitter) : spec_(spec), jitter_(jitter) {
    }

public:
    CronSpec spec() const {
        return spec_;
    }

public:
    void run() override;
    bool valid() const override;
    bool enabled() const override;
    uint32_t getNextTime(DateTime after, uint32_t seed) const override;
    void accept(TaskVisitor &visitor) override {
        visitor.visit(*this);
    }

};

class Scheduler {
private:
    Task **tasks_{ nullptr };
    size_t size_{ 0 };
    uint32_t last_now_{ 0 };

public:
    Scheduler() {
    }

    template<size_t N>
    Scheduler(Task* (&tasks)[N]) : tasks_(&tasks[0]), size_(N) {
    }

public:
    size_t size() const {
        return size_;
    }

    Task *get(size_t i) const {
        return tasks_[i];
    }

    Task *operator[] (size_t i) const {
        return tasks_[i];
    }

public:
    void accept(TaskVisitor &visitor) {
        for (size_t i = 0; i < size_; ++i) {
            tasks_[i]->accept(visitor);
        }
    }

public:
    struct TaskAndTime {
        uint32_t time{ 0 };
        Task *task{ nullptr };

        TaskAndTime() {
        }

        TaskAndTime(uint32_t time, Task *task) : time(time), task(task) {
        }

        operator bool() const {
            return task != nullptr;
        }
    };

    void begin(DateTime now);

    TaskAndTime check(DateTime now, uint32_t seed = 0);

    TaskAndTime nextTask(DateTime now, uint32_t seed = 0);

    TaskAndTime nextTask();
};

}

#endif
