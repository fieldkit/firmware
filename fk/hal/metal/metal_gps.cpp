#include "hal/metal/metal_gps.h"
#include "board.h"
#include "platform.h"

#if defined(ARDUINO)

#include <Arduino.h>

namespace fk {

FK_DECLARE_LOGGER("gps");

bool MetalGps::begin() {
    Serial1.begin(9600);

    board.enable_gps();

    return true;
}

bool MetalGps::check() {
    return true;
}

bool MetalGps::service(GpsFix &fix) {
    char buffer[128];
    size_t i = 0;

    while (true) {
        if (Serial1.available()) {
            char c = Serial1.read();
            gps_.encode(c);

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
        fk_delay(10);
    }

    return true;
}

}

#endif
