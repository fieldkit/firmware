#if defined(__SAMD51__)
#include "hal/metal/metal_gps.h"
namespace fk {
static MetalGps gps;
}
#else
#include "hal/linux/linux_gps.h"
namespace fk {
static LinuxGps gps;
}
#endif

namespace fk {

Gps *get_gps() {
    return &gps;
}

}
