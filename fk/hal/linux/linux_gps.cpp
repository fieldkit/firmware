#include "hal/linux/linux_gps.h"

namespace fk {

bool LinuxGps::begin() {
    return true;
}

bool LinuxGps::service(GpsFix &fix) {
    return true;
}

bool LinuxGps::stop() {
    return true;
}

}
