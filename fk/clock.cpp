#include <compiler.h>
#include <hal_init.h>
#include <hal/include/hpl_calendar.h>
#include <hal/include/hal_calendar.h>

#include "clock.h"

namespace fk {

FK_DECLARE_LOGGER("clock");

struct calendar_descriptor CALENDAR_0;

static void CALENDAR_0_initialize(void) {
    hri_mclk_set_APBAMASK_RTC_bit(MCLK);
    calendar_init(&CALENDAR_0, RTC);
}

static uint8_t bcd2bin(uint8_t val) {
    return val - 6 * (val >> 4);
}

static uint8_t bin2bcd(uint8_t val) {
    return val + 6 * (val / 10);
}

CoreClock::CoreClock(TwoWire &wire) : wire_(&wire) {
}

bool CoreClock::begin() {
    CALENDAR_0_initialize();
    calendar_enable(&CALENDAR_0);

    wire_->begin();
    DateTime time;
    return now(time);
}

bool CoreClock::adjust(DateTime now) {
    struct calendar_date date;
    struct calendar_time time;

    time.hour = now.hour();
    time.min = now.minute();
    time.sec = now.second();

    calendar_set_time(&CALENDAR_0, &time);

    date.year = now.year();
    date.month = now.month();
    date.day = now.day();

    calendar_set_date(&CALENDAR_0, &date);

    wire_->beginTransmission(Address);
    wire_->write(0x26);
    wire_->requestFrom(Address, 1);
    auto bsm = wire_->read();
    if (!I2C_CHECK(wire_->endTransmission())) {
        return false;
    }

    if (bsm != 0) {
        loginfo("fixing battery switch mode = 0x%x", bsm);
        wire_->beginTransmission(Address);
        wire_->write(0x26);
        wire_->write(0x00);
        if (!I2C_CHECK(wire_->endTransmission())) {
            return false;
        }
    }

    wire_->beginTransmission(Address);
    wire_->write(0x00);
    wire_->write(0);
    wire_->write(bin2bcd(now.second()));
    wire_->write(bin2bcd(now.minute()));
    wire_->write(bin2bcd(now.hour()));
    wire_->write(bin2bcd(now.day()));
    wire_->write(bin2bcd(0));
    wire_->write(bin2bcd(now.month()));
    wire_->write(bin2bcd(now.year() - 2000));
    if (!I2C_CHECK(wire_->endTransmission())) {
        return false;
    }

    return true;
}

bool CoreClock::now(DateTime &time) {
    uint8_t data[8];

    if (true) {
        wire_->beginTransmission(Address);
        wire_->write(0x00);
        wire_->requestFrom(Address, sizeof(data));
        for (auto i = (size_t)0; i < sizeof(data); ++i) {
            data[i] = wire_->read();
        }
        if (!I2C_CHECK(wire_->endTransmission())) {
            return false;
        }

        auto os_flag = data[1] & B10000000;
        if (os_flag) {
            loginfo("possible accuracy error!");
        }

        time = DateTime{
            (uint16_t)(bcd2bin(data[7]) + 2000),
            bcd2bin(data[6] & B00011111),
            bcd2bin(data[4] & B00111111),
            bcd2bin(data[3] & B00111111),
            bcd2bin(data[2] & B01111111),
            bcd2bin(data[1] & B01111111)
        };
    }
    else {
        struct calendar_date_time mcu_time;
        calendar_get_date_time(&CALENDAR_0, &mcu_time);

        time = DateTime{
            mcu_time.date.year,
            mcu_time.date.month,
            mcu_time.date.day,
            mcu_time.time.hour,
            mcu_time.time.min,
            mcu_time.time.sec,
        };
    }

    return true;
}

DateTime CoreClock::now() {
    DateTime time;
    now(time);
    return time;
}

void CoreClock::read_timestamp_registers() {
    uint8_t data[6 * 3];

    wire_->beginTransmission(Address);
    wire_->write(0x11);
    wire_->requestFrom(Address, sizeof(data));
    for (auto i = (size_t)0; i < sizeof(data); ++i) {
        data[i] = wire_->read();
    }
    wire_->endTransmission();

    log_tsr(&data[6 * 0]);
    log_tsr(&data[6 * 1]);
    log_tsr(&data[6 * 2]);
}

void CoreClock::clear_timestamp_registers() {
    loginfo("clearing TSR (seems broken)");

    wire_->beginTransmission(Address);
    wire_->write(0x2f);
    wire_->write(0x25);
    wire_->endTransmission();
}

void CoreClock::log_tsr(uint8_t *ts) {
    loginfo("tsr: %04d/%02d/%02d %02d:%02d:%02d.%d",
            bcd2bin(ts[6]) + 2000,
            bcd2bin(ts[5] & B00011111),
            bcd2bin(ts[4] & B00111111),
            bcd2bin(ts[3] & B00111111),
            bcd2bin(ts[2] & B01111111),
            bcd2bin(ts[1] & B01111111),
            bcd2bin(ts[0])
        );
}

}
