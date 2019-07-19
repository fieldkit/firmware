#include "network.h"

#include "hal/metal/metal_network.h"
#include "hal/linux/linux_network.h"

namespace fk {

#if defined(FK_HARDWARE_FULL)
static MetalNetwork network;
#else
static LinuxNetwork network;
#endif

Network *get_network() {
    return &network;
}

}
