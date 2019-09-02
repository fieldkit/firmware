#include <tiny_printf.h>
#include <algorithm>

#include "lora/lora.h"
#include "hal/board.h"
#include "sc16is740.h"
#include "rn2903.h"

#if defined(ARDUINO)

#include <Arduino.h>

namespace fk {

FK_DECLARE_LOGGER("lora");

enum class LoraState {
    Initializing,
    Waiting,
};

LoraModule::LoraModule() {
}

bool LoraModule::begin() {
    pinMode(LORA_ENABLE, OUTPUT);
    digitalWrite(LORA_ENABLE, LOW);
    fk_delay(500);
    digitalWrite(LORA_ENABLE, HIGH);
    fk_delay(100);

    auto bus = get_board()->i2c_radio();

    Rn2903 rn2903{ bus };
    if (!rn2903.begin()) {
        return false;
    }

    const char *line = nullptr;
    if (!rn2903.read_line_sync(&line, 5000)) {
        return false;
    }

    const char *dev_eui = "008C1F21E6162E58";
    if (!rn2903.simple_query("mac set deveui %s", &line, 1000, dev_eui)) {
        return false;
    }

    if (!rn2903.simple_query("sys get hweui", &line, 1000)) {
        return false;
    }
    if (!rn2903.simple_query("sys get vdd", &line, 1000)) {
        return false;
    }
    if (!rn2903.simple_query("mac get appeui", &line, 1000)) {
        return false;
    }
    if (!rn2903.simple_query("mac get deveui", &line, 1000)) {
        return false;
    }
    if (!rn2903.simple_query("mac get dr", &line, 1000)) {
        return false;
    }
    if (!rn2903.simple_query("mac get rxdelay1", &line, 1000)) {
        return false;
    }
    if (!rn2903.simple_query("mac get rxdelay2", &line, 1000)) {
        return false;
    }

    const char *app_eui = "70B3D57ED001F439";
    const char *app_key = "900AA739F7AA08D083CDE23E96FA0820";

    while (true) {
        if (rn2903.join(app_eui, app_key)) {
            break;
        }

        fk_delay(5000);
    }

    return true;
}

}

#endif
