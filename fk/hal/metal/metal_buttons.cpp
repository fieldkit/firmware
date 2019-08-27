#include "hal/metal/metal.h"

#if defined(ARDUINO)

namespace fk {

FK_DECLARE_LOGGER("buttons");

static void irq_button_left() {
    auto down = digitalRead(BUTTON_LEFT) == LOW;
    reinterpret_cast<MetalButtons*>(get_buttons())->irq(Buttons::Left, down);
}

static void irq_button_middle() {
    auto down = digitalRead(BUTTON_MIDDLE) == LOW;
    reinterpret_cast<MetalButtons*>(get_buttons())->irq(Buttons::Middle, down);
}

static void irq_button_right() {
    auto down = digitalRead(BUTTON_RIGHT) == LOW;
    reinterpret_cast<MetalButtons*>(get_buttons())->irq(Buttons::Right, down);
}

MetalButtons::MetalButtons() {
}

bool MetalButtons::begin() {
    struct setup_t {
        uint8_t pin;
        uint8_t index;
        void (*irq_handler)(void);
    };

    setup_t setups[] = {
        { BUTTON_LEFT, Buttons::Left, irq_button_left },
        { BUTTON_MIDDLE, Buttons::Middle, irq_button_middle },
        { BUTTON_RIGHT, Buttons::Right, irq_button_right },
    };

    for (auto &s : setups) {
        pinMode(s.pin, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(s.pin), s.irq_handler, CHANGE);
        if (digitalRead(s.pin) == LOW) {
            buttons_[s.index].changed(true);
        }
    }

    return true;
}

uint8_t MetalButtons::number_pressed() const {
    auto n = 0;

    for (auto i = 0; i < NumberOfButtons; ++i) {
        if (buttons_[i].down()) {
            n++;
        }
    }

    return n;
}

bool MetalButtons::get(uint8_t which) const {
    switch (which) {
    case Left: return digitalRead(BUTTON_LEFT) == LOW;
    case Middle: return digitalRead(BUTTON_MIDDLE) == LOW;
    case Right: return digitalRead(BUTTON_RIGHT) == LOW;
    }
    return false;
}

void MetalButtons::irq(uint8_t index, bool down) {
    buttons_[index].changed(down);
}

}

#endif // defined(ARDUINO)
