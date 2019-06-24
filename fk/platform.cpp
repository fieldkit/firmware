#include "fk.h"
#include "platform.h"
#include <Arduino.h>

namespace fk {

uint32_t fk_uptime() {
    return millis();
}

}
