#include "hal/linux/linux_leds.h"

namespace fk {

LinuxLeds::LinuxLeds() {
}

bool LinuxLeds::begin() {
    return true;
}

void LinuxLeds::brightness(uint8_t value) {
}

void LinuxLeds::off() {
}

void LinuxLeds::on() {
}

} // namespace fk
