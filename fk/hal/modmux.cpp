#include "hal/hal.h"
#include "hal/metal/metal.h"
#include "hal/linux/linux.h"

namespace fk {

FK_DECLARE_LOGGER("modmux");

#if defined(FK_HARDWARE_FULL)
MetalModMux mm;
#else
LinuxModMux mm;
#endif

ModMux *get_modmux() {
    return &mm;
}

DisableModuleOnReturn::DisableModuleOnReturn(uint8_t position) : position_(position) { 
    if (!get_modmux()->enable_module(position_)) {
        logerror("[%d] enable failed", position_);
    }
}

DisableModuleOnReturn::~DisableModuleOnReturn() {
    if (!get_modmux()->disable_module(position_)) {
        logerror("[%d] disable failed", position_);
    }
}

}
