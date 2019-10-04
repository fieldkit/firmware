#include "hal/linux/linux.h"

namespace fk {

LinuxSdCard::LinuxSdCard() {
}

bool LinuxSdCard::begin() {
    return true;
}

bool LinuxSdCard::append_logs(circular_buffer<char> &buffer) {
    return true;
}

}
