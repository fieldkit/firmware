#include "sd_card.h"

#include "hal/metal/metal_sd_card.h"
#include "hal/linux/linux_sd_card.h"

namespace fk {

#if defined(FK_HARDWARE_FULL)
static MetalSdCard sd_card;
#else
static LinuxSdCard sd_card;
#endif

SdCard *get_sd_card() {
    return &sd_card;
}

}
