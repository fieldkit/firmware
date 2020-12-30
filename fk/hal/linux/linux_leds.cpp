#include "hal/linux/linux_leds.h"

namespace fk {

LinuxLeds::LinuxLeds() {
}

bool LinuxLeds::begin() {
    return true;
}

bool LinuxLeds::brightness(uint8_t value, bool refresh) {
    return false;
}

void LinuxLeds::off() {
}

void LinuxLeds::on() {
}

void LinuxLeds::color(uint8_t position, Color color, bool refresh) {
}

void LinuxLeds::off(uint8_t position) {
}

void LinuxLeds::on(uint8_t position) {
}

bool LinuxLeds::refresh() {
    return true;
}

} // namespace fk
