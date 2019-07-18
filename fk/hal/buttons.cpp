#include "hal/buttons.h"
#include "hal/metal/metal_buttons.h"
#include "hal/linux/linux_buttons.h"

namespace fk {

FK_DECLARE_LOGGER("buttons");

Button::Button(const char *name) : name_(name) {
}

void Button::changed(uint8_t pin, bool down) {
    if (down) {
        down_ = true;
        time_ = fk_uptime();
    }
    else if (down_) {
        auto elapsed = fk_uptime() - time_;
        down_ = false;
        time_ = 0;
        loginfo("%d (%dms)", pin, elapsed);

        if (!get_ipc()->enqueue(this, 0)) {
            logerror("ipc error");
        }
    }
}

#if defined(FK_HARDWARE_FULL)
static MetalButtons buttons;
#else
static LinuxButtons buttons;
#endif

Buttons *get_buttons() {
    return &buttons;
}

}
