#include "hal/linux/linux.h"

namespace fk {

LinuxButtons::LinuxButtons() {
}

bool LinuxButtons::begin() {
    return true;
}

bool LinuxButtons::is_debug_pressed() const {
    return false;
}

bool LinuxButtons::get(uint8_t which) const {
    return false;
}

}
