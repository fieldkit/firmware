#include "hal/lora.h"

#if defined(FK_HARDWARE_FULL)
#include "hal/metal/metal_lora.h"
#endif

namespace fk {

#if defined(FK_HARDWARE_FULL)
static Rn2903LoraNetwork network;
#else
static NoopLoraNetwork network;
#endif

LoraNetwork *get_lora_network() {
    return &network;
}

}
