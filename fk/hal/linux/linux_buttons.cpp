#include "hal/linux/linux.h"

namespace fk {

LinuxButtons::LinuxButtons() {
}

bool LinuxButtons::begin() {
    return true;
}

uint8_t LinuxButtons::number_pressed() const {
    return 0;
}

}
