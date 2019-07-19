#include "hal/metal/metal.h"

#if defined(ARDUINO)

namespace fk {

FK_DECLARE_LOGGER("buttons");

static void irq_button_left() {
    reinterpret_cast<MetalButtons*>(get_buttons())->irq(BUTTON_LEFT, Buttons::Left);
}

static void irq_button_middle() {
    reinterpret_cast<MetalButtons*>(get_buttons())->irq(BUTTON_MIDDLE, Buttons::Middle);
}

static void irq_button_right() {
    reinterpret_cast<MetalButtons*>(get_buttons())->irq(BUTTON_RIGHT, Buttons::Right);
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

void MetalButtons::irq(uint8_t pin, uint8_t index) {
    auto down = digitalRead(pin) == LOW;
    auto &b = buttons_[index];
    b.changed(down);
}

}

#endif // defined(ARDUINO)
