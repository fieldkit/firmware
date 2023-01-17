#include "hal/hal.h"
#include "hal/metal/metal.h"
#include "hal/clock.h"
#include "platform.h"
#include "config.h"

#if defined(__SAMD51__)

#include <Arduino.h>

namespace fk {

FK_DECLARE_LOGGER("gps");

constexpr const char *PGCMD_ANTENNA = "$PGCMD,33,1*6C";
constexpr const char *PMTK_SET_NMEA_UPDATE_1HZ = "$PMTK220,1000*1F";
constexpr const char *PMTK_API_SET_FIX_CTL_1HZ = "$PMTK300,1000,0,0,0,0*1C";

bool MetalGps::begin() {
    Serial1.begin(9600);

    get_board()->enable_gps();

    Serial1.println(PGCMD_ANTENNA);
    Serial1.println(PMTK_SET_NMEA_UPDATE_1HZ);
    Serial1.println(PMTK_API_SET_FIX_CTL_1HZ);

    gps_ = {};

    return true;
}

bool MetalGps::service(GpsFix &fix) {
    struct time_args_t {
        uint32_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        uint8_t hundredths;
        uint32_t time_fix_age;
        uint32_t date;
        uint32_t time;
    } time;

    while (Serial1.available()) {
        char c = Serial1.read();
        gps_.encode(c);

        if (GpsLoggingRaw) {
            if (position_ == sizeof(buffer_) - 1 || c == '\n') {
                if (position_ > 0) {
                    loginfo("%s", buffer_);
                }
                position_ = 0;
            } else {
                buffer_[position_++] = c;
                buffer_[position_] = 0;
            }
        }
    }

    fix.altitude = gps_.f_altitude();
    fix.hdop = gps_.hdop();

    gps_.f_get_position(&fix.latitude, &fix.longitude, &fix.position_fix_age);
    gps_.crack_datetime((int *)&time.year, &time.month, &time.day, &time.hour, &time.minute, &time.second, &time.hundredths,
                        &time.time_fix_age);
    gps_.get_datetime(&time.date, &time.time, &time.time_fix_age);

    auto valid = true;

    if (fix.hdop == TinyGPS::GPS_INVALID_HDOP) {
        valid = false;
    }

    if (fix.longitude == TinyGPS::GPS_INVALID_ANGLE || fix.latitude == TinyGPS::GPS_INVALID_ANGLE) {
        valid = false;
    }

    if (time.date != TinyGPS::GPS_INVALID_DATE && time.time != TinyGPS::GPS_INVALID_TIME) {
        auto now = DateTime(time.year, time.month, time.day, time.hour, time.minute, time.second);
        fix.time = now.unix_time();
        fix.time_fix_age = time.time_fix_age;
    } else {
        valid = false;
    }

    if (fix.time == 0) {
        valid = false;
    }

    if (valid) {
        fix.valid = true;
    } else {
        fix = {};
    }

    // Do this after, so that these are included even if we have
    // an invalid fix.
    gps_.stats(&fix.chars, &fix.good, &fix.failed);

    // Also always include number of satellites, for diagnostics.
    auto satellites = gps_.satellites();
    if (satellites != TinyGPS::GPS_INVALID_SATELLITES) {
        fix.satellites = satellites;
    }

    return true;
}

bool MetalGps::stop() {
    Serial1.end();

    get_board()->disable_gps();

    gps_ = {};

    return true;
}

} // namespace fk

#endif
