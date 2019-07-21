#include "hal/hal.h"
#include "hal/linux/linux.h"

#if defined(linux)

namespace fk {

LinuxModMux::LinuxModMux() {
}

bool LinuxModMux::begin() {
    return true;
}

bool LinuxModMux::enable_all_modules() {
    return true;
}

bool LinuxModMux::disable_all_modules() {
    return true;
}

}

#endif
