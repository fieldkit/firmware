#if defined(ARDUINO)
#include <compiler.h>
#include <hal_init.h>
#include <hal/include/hpl_calendar.h>
#include <hal/include/hal_calendar.h>
#else
#include <ctime>
#endif

#include "clock.h"

namespace fk {

#if defined(ARDUINO)

#define CTRL_OFFSET           0x24
#define CTRL_OSCILLATOR       0x25
#define CTRL_BATTERY          0x26
#define CTRL_PIN_IO           0x27
#define CTRL_FUNCTION         0x28
#define CTRL_INTA_EN          0x29
#define CTRL_INTB_EN          0x2a
#define CTRL_FLAGS            0x2b
#define CTRL_RAMBYTE          0x2c
#define CTRL_WDOG             0x2d
#define CTRL_STOP_EN          0x2e
#define CTRL_RESETS           0x2f
#define CTRL_RAM              0x40

#define FLAGS_TSR1F	          BIT(0)
#define FLAGS_TSR2F	          BIT(1)
#define FLAGS_TSR3F	          BIT(2)
#define FLAGS_BSF             BIT(3)
#define FLAGS_WDF             BIT(4)
#define FLAGS_A1F             BIT(5)
#define FLAGS_A2F             BIT(6)
#define FLAGS_PIF             BIT(7)

#define NVRAM_SIZE	          0x40
#define RESET_CPR	            0xa4
#define STOP_EN_STOP	        BIT(0)

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

CoreClock::CoreClock() : wire_{ "fake", nullptr } {
}

CoreClock::CoreClock(TwoWireWrapper wire) : wire_(wire) {
}

bool CoreClock::begin() {
    CALENDAR_0_initialize();
    calendar_enable(&CALENDAR_0);

    if (!configure()) {
        return false;
    }

    return sync();
}

bool CoreClock::configure() {
    if (configured_) {
        return true;
    }

    uint8_t bsm;
    if (!I2C_CHECK(wire_.read_register_u8(Address, 0x26, bsm))) {
        return false;
    }

    if (bsm != 0) {
        loginfo("fixing battery switch mode = 0x%x", bsm);
        if (!I2C_CHECK(wire_.write_register_u8(Address, 0x26, 0x00))) {
            return false;
        }
    }

    configured_ = true;

    return true;
}

bool CoreClock::sync() {
    DateTime trusted;
    if (!external(trusted)) {
        return false;
    }

    FormattedTime formatted{ trusted.unix_time() };
    loginfo("%s (%d)", formatted.cstr(), trusted.unix_time());

    // Set the internal time from our trusted, external source.
    struct calendar_time time;
    time.hour = trusted.hour();
    time.min = trusted.minute();
    time.sec = trusted.second();

    calendar_set_time(&CALENDAR_0, &time);

    struct calendar_date date;
    date.year = trusted.year();
    date.month = trusted.month();
    date.day = trusted.day();

    calendar_set_date(&CALENDAR_0, &date);

    return true;
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

    uint8_t adjust_command[] = {
        CTRL_STOP_EN,
        STOP_EN_STOP,
        RESET_CPR,
        0,
        bin2bcd(now.second()),
        bin2bcd(now.minute()),
        bin2bcd(now.hour()),
        bin2bcd(now.day()),
        bin2bcd(0),
        bin2bcd(now.month()),
        bin2bcd(now.year() - 2000),
    };
    if (!I2C_CHECK(wire_.write(Address, &adjust_command, sizeof(adjust_command)))) {
        return false;
    }

    uint8_t resume_command[] = {
        CTRL_STOP_EN,
        0,
    };
    if (!I2C_CHECK(wire_.write(Address, &resume_command, sizeof(resume_command)))) {
        return false;
    }

    return true;
}

bool CoreClock::internal(DateTime &time) {
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

    return true;
}

bool CoreClock::external(DateTime &time) {
    uint8_t data[8];
    if (!I2C_CHECK(wire_.read_register_buffer(Address, 0x00, data, sizeof(data)))) {
        return false;
    }

    auto os_flag = data[1] & 0b10000000;
    if (os_flag) {
        loginfo("possible accuracy error!");
    }

    time = DateTime{
        (uint16_t)(bcd2bin(data[7]) + 2000),
        bcd2bin(data[6] & 0b00011111),
        bcd2bin(data[4] & 0b00111111),
        bcd2bin(data[3] & 0b00111111),
        bcd2bin(data[2] & 0b01111111),
        bcd2bin(data[1] & 0b01111111)
    };

    return true;
}

DateTime CoreClock::now() {
    DateTime time;
    if (!internal(time)) {
        logwarn("error getting internal time");
    }
    return time;
}

DateTime CoreClock::get_external() {
    DateTime time;
    if (!external(time)) {
        logwarn("error getting external time");
    }
    return time;
}

void CoreClock::read_timestamp_registers() {
    uint8_t data[6 * 3];
    if (!I2C_CHECK(wire_.read_register_buffer(Address, 0x11, data, sizeof(data)))) {
        return;
    }

    log_tsr(&data[6 * 0]);
    log_tsr(&data[6 * 1]);
    log_tsr(&data[6 * 2]);
}

void CoreClock::clear_timestamp_registers() {
    loginfo("clearing TSR (seems broken)");

    wire_.write_register_u8(Address, 0x2f, 0x25);
}

void CoreClock::log_tsr(uint8_t *ts) {
    loginfo("tsr: %04d/%02d/%02d %02d:%02d:%02d.%d",
            bcd2bin(ts[6]) + 2000,
            bcd2bin(ts[5] & 0b00011111),
            bcd2bin(ts[4] & 0b00111111),
            bcd2bin(ts[3] & 0b00111111),
            bcd2bin(ts[2] & 0b01111111),
            bcd2bin(ts[1] & 0b01111111),
            bcd2bin(ts[0])
        );
}

static bool valid = false;
static CoreClock clock;

CoreClock *get_clock() {
    if (!valid) {
        clock = CoreClock{ get_board()->i2c_core() };
        valid = true;
    }
    return &clock;
}

uint32_t get_clock_now() {
    return get_clock()->now().unix_time();
}

#else

uint32_t get_clock_now() {
    return std::time(0);
}

#endif

}
