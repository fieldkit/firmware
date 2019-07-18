#include "hal/metal/metal.h"

#if defined(ARDUINO)

namespace fk {

FK_DECLARE_LOGGER("buttons");

static void irq_button_right() {
    reinterpret_cast<MetalButtons*>(get_buttons())->irq(BUTTON_RIGHT, 0);
}

static void irq_button_middle() {
    reinterpret_cast<MetalButtons*>(get_buttons())->irq(BUTTON_MIDDLE, 1);
}

static void irq_button_left() {
    reinterpret_cast<MetalButtons*>(get_buttons())->irq(BUTTON_LEFT, 2);
}

MetalButtons::MetalButtons() {
}

bool MetalButtons::begin() {
    pinMode(BUTTON_RIGHT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_RIGHT), irq_button_right, CHANGE);

    pinMode(BUTTON_MIDDLE, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_MIDDLE), irq_button_middle, CHANGE);

    pinMode(BUTTON_LEFT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_LEFT), irq_button_left, CHANGE);

    return true;
}

void MetalButtons::irq(uint8_t pin, uint8_t index) {
    auto down = digitalRead(pin) == LOW;
    auto &b = buttons_[index];
    b.changed(pin, down);
}

}

#endif // defined(ARDUINO)
