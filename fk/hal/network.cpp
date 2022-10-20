#include "network.h"

#include "hal/metal/metal_network.h"
#include "hal/metal/winc1500_network.h"
#include "hal/metal/esp32_network.h"
#include "hal/linux/linux_network.h"

namespace fk {

#if defined(FK_HARDWARE_FULL)
#if defined(FK_NETWORK_ESP32)
static Esp32Network network;
#else
static Winc1500Network network;
#endif
#else
static LinuxNetwork network;
#endif

Network *get_network() {
    return &network;
}

} // namespace fk
