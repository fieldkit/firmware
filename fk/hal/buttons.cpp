#include "hal/buttons.h"
#include "hal/metal/metal_buttons.h"
#include "hal/linux/linux_buttons.h"

namespace fk {

FK_DECLARE_LOGGER("buttons");

constexpr uint32_t ButtonDebounceDelay = 50;

Button::Button(const char *name, uint8_t index) : name_(name), index_(index) {
}

void Button::changed(bool down) {
    auto now = fk_uptime();
    if (debounce_ > 0 && now < debounce_) {
        return;
    }
    if (down) {
        if (!down_) {
            down_ = true;
            time_ = now;
            debounce_ = now + ButtonDebounceDelay;
        }
    }
    else if (down_) {
        auto elapsed = now - time_;
        down_ = false;
        time_ = 0;
        debounce_ = now + ButtonDebounceDelay;
        pressed_ = now;
        loginfo("%s (%" PRIu32 "ms)", name_, elapsed);
        if (get_ipc()->available()) {
            if (!get_ipc()->enqueue_button(this)) {
                logerror("ipc error (button)");
            }
            if (!get_ipc()->enqueue_activity(this)) {
                logerror("ipc error (activity)");
            }
            /*
            if (!get_ipc()->enqueue_data([](GlobalState *gs) {
                gs->runtime.activity = fk_uptime();
            })) {
                logerror("ipc error (data)");
            }
            */
        }
    }
}

bool Button::is_up() const {
    return index_ == Buttons::Left;
}

bool Button::is_enter() const {
    return index_ == Buttons::Middle;
}

bool Button::is_down() const {
    return index_ == Buttons::Right;
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
