#include "hal.h"
#include "hal/metal/metal_ipc.h"
#include "hal/linux/linux_ipc.h"

namespace fk {

#if defined(FK_HARDWARE_FULL)
static MetalIPC ipc;
#else
static LinuxIPC ipc;
#endif

IPC *get_ipc() {
    return &ipc;
}

}
