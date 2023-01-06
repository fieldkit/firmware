#include "hal/buttons.h"
#include "hal/metal/metal_buttons.h"
#include "hal/linux/linux_buttons.h"
#include "tasks/tasks.h"

namespace fk {

FK_DECLARE_LOGGER("buttons");

constexpr uint32_t ButtonDebounceDelay = 50;

Button::Button(const char *name, uint8_t index) : name_(name), index_(index) {
}

void Button::changed(bool down) {
    auto now = fk_uptime();
    if (debounce_ > 0 && now - debounce_ < ButtonDebounceDelay) {
        logdebug("%s debounce", name_);
        return;
    }

    auto debug_mode = fk_debug_mode();
    auto display_running = os_task_is_running(&display_task);
    if (down) {
        // Logging here can increase latency so much that testing button related
        // things becomes pointless.
        if (!down_) {
            down_ = true;
            time_ = now;
            debounce_ = now;
            had_display_ = display_running;
            if (!debug_mode || !display_running) {
                if (!get_ipc()->enqueue_activity(this)) {
                    logerror("ipc error (activity)");
                }
            }
        }
    } else if (down_) {
        auto elapsed = now - time_;
        down_ = false;
        time_ = 0;
        debounce_ = now;
        pressed_ = now;

        auto available = get_ipc()->available();
        loginfo("%s (%" PRIu32 "ms) (ipc=%d) (debug=%d) (display=%d/%d)", name_, elapsed, available, debug_mode, had_display_,
                display_running);
        if (available) {
            if (had_display_) {
                if (!get_ipc()->enqueue_button(this)) {
                    logerror("ipc error (button)");
                }
            }
            if (!debug_mode) {
                if (!get_ipc()->enqueue_activity(this)) {
                    logerror("ipc error (activity)");
                }
            }
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

bool Button::is_external() const {
    return index_ == Buttons::External;
}

#if defined(FK_HARDWARE_FULL)
static MetalButtons buttons;
#else
static LinuxButtons buttons;
#endif

Buttons *get_buttons() {
    return &buttons;
}

} // namespace fk
