#include "hal/metal/metal.h"

#if defined(__SAMD51__)

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

static void irq_button_external() {
    auto down = digitalRead(BUTTON_EXTERNAL) == LOW;
    reinterpret_cast<MetalButtons*>(get_buttons())->irq(Buttons::External, down);
}

MetalButtons::MetalButtons() {
}

bool MetalButtons::begin() {
    struct setup_t {
        uint8_t pin;
        uint8_t index;
        void (*irq_handler)(void);
        IRQn irq;
    };

    setup_t setups[] = {
#if defined(FK_UNDERWATER)
        // Would love to DRY this up with the IRQs defined in the Arduino table.
        { BUTTON_LEFT, Buttons::Left, irq_button_left, EIC_0_IRQn },
        { BUTTON_MIDDLE, Buttons::Middle, irq_button_middle, EIC_1_IRQn },
        { BUTTON_RIGHT, Buttons::Right, irq_button_right, EIC_2_IRQn },
        { BUTTON_EXTERNAL, Buttons::External, irq_button_external, EIC_0_IRQn },
#else
        { BUTTON_LEFT, Buttons::Left, irq_button_left, EIC_7_IRQn },
        { BUTTON_MIDDLE, Buttons::Middle, irq_button_middle, EIC_6_IRQn },
        { BUTTON_RIGHT, Buttons::Right, irq_button_right, EIC_5_IRQn },
        { BUTTON_EXTERNAL, Buttons::External, irq_button_external, EIC_4_IRQn },
#endif
    };

    for (auto &s : setups) {
        pinMode(s.pin, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(s.pin), s.irq_handler, CHANGE);
        NVIC_SetPriority(s.irq, FK_PRIORITY_BUTTONS);
        if (digitalRead(s.pin) == LOW) {
            buttons_[s.index].changed(true);
        }
    }

    return true;
}

bool MetalButtons::is_debug_pressed() const {
    return buttons_[Buttons::Right].down();
}

bool MetalButtons::get(uint8_t which) const {
    switch (which) {
    case Left: return digitalRead(BUTTON_LEFT) == LOW;
    case Middle: return digitalRead(BUTTON_MIDDLE) == LOW;
    case Right: return digitalRead(BUTTON_RIGHT) == LOW;
    case External: return digitalRead(BUTTON_EXTERNAL) == LOW;
    }
    return false;
}

void MetalButtons::irq(uint8_t index, bool down) {
    buttons_[index].changed(down);
}

}

#endif // defined(__SAMD51__)
