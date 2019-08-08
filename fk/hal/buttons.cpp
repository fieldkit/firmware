#include "hal/buttons.h"
#include "hal/metal/metal_buttons.h"
#include "hal/linux/linux_buttons.h"

namespace fk {

FK_DECLARE_LOGGER("buttons");

Button::Button(const char *name) : name_(name) {
}

void Button::changed(bool down) {
    if (down) {
        down_ = true;
        time_ = fk_uptime();
        loginfo("%s (DOWN)", name_);
    }
    else if (down_) {
        auto elapsed = fk_uptime() - time_;
        down_ = false;
        time_ = 0;
        loginfo("%s (%" PRIu32 "ms)", name_, elapsed);
        if (get_ipc()->available()) {
            if (!get_ipc()->enqueue(this, 0)) {
                logerror("ipc error");
            }
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
