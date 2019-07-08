#include "hal/metal/metal_gps.h"
#include "board.h"
#include "platform.h"

#if defined(ARDUINO)

#include <RTClib.h>
#include <Arduino.h>

namespace fk {

FK_DECLARE_LOGGER("gps");

constexpr const char *PGCMD_ANTENNA = "$PGCMD,33,1*6C";
constexpr const char *PMTK_SET_NMEA_UPDATE_1HZ = "$PMTK220,1000*1F";
constexpr const char *PMTK_API_SET_FIX_CTL_1HZ = "$PMTK300,1000,0,0,0,0*1C";

bool MetalGps::begin() {
    Serial1.begin(9600);

    board.enable_gps();

    Serial1.println(PGCMD_ANTENNA);
    Serial1.println(PMTK_SET_NMEA_UPDATE_1HZ);
    Serial1.println(PMTK_API_SET_FIX_CTL_1HZ);

    return true;
}

bool MetalGps::check() {
    return true;
}

bool MetalGps::service(GpsFix &fix) {
    char buffer[64];
    size_t i = 0;

    gps_ = TinyGPS();

    uint32_t position_fix_age;
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

    while (true) {
        if (Serial1.available()) {
            char c = Serial1.read();
            gps_.encode(c);

            fix.satellites = gps_.satellites();
            fix.altitude = gps_.f_altitude();
            fix.hdop = gps_.hdop();
            gps_.f_get_position(&fix.latitude, &fix.longitude, &position_fix_age);
            gps_.crack_datetime((int *)&time.year, &time.month, &time.day, &time.hour, &time.minute, &time.second, &time.hundredths, &time.time_fix_age);
            gps_.get_datetime(&time.date, &time.time, &time.time_fix_age);

            auto ok = true;

            if (fix.longitude != TinyGPS::GPS_INVALID_ANGLE && fix.latitude != TinyGPS::GPS_INVALID_ANGLE) {

            }
            else {
                ok = false;
            }

            if (time.date != TinyGPS::GPS_INVALID_DATE && time.time != TinyGPS::GPS_INVALID_TIME) {
                auto now = DateTime(time.year, time.month, time.day, time.hour, time.minute, time.second);
                fix.time = now.unixtime();
            }
            else {
                ok = false;
            }

            fix.valid = ok;

            if (false) {
                if (i == sizeof(buffer) - 1 || c == '\n') {
                    if (i > 0) {
                        logtrace("%s", buffer);
                    }
                    i = 0;
                }
                else {
                    buffer[i++] = c;
                    buffer[i] = 0;
                }
            }
        }
        fk_delay(10);
    }

    return true;
}

}

#endif
