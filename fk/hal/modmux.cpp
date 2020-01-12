#include "hal/hal.h"
#include "hal/metal/metal.h"
#include "hal/linux/linux.h"

namespace fk {

#if defined(FK_HARDWARE_FULL)
MetalModMux mm;
#else
LinuxModMux mm;
#endif

ModMux *get_modmux() {
    return &mm;
}

}
