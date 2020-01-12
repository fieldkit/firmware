#include "hal/leds.h"
#include "hal/metal/metal.h"
#include "hal/linux/linux.h"

namespace fk {

ModuleLeds::ModuleLeds() {
}

ModuleLeds::~ModuleLeds() {
}

#if defined(FK_HARDWARE_FULL)
MetalLeds module_leds;
#else
LinuxLeds module_leds;
#endif

ModuleLeds *get_module_leds() {
    return &module_leds;
}

}
