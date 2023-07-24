#include "hal/lora.h"

#if !defined(FK_DISABLE_LORA) && defined(FK_HARDWARE_FULL)
#include "hal/metal/metal_lora.h"
#endif

namespace fk {

#if !defined(FK_DISABLE_LORA) && defined(FK_HARDWARE_FULL)
static TheThingsLoraNetwork lora_network;
#else
static NoopLoraNetwork lora_network;
#endif

LoraNetwork *get_lora_network() {
    return &lora_network;
}

} // namespace fk