#include "hal/leds.h"
#include "hal/metal/metal.h"
#include "hal/linux/linux.h"

namespace fk {

ModuleLeds::ModuleLeds() {
}

ModuleLeds::~ModuleLeds() {
}

class NoopModuleLeds : public ModuleLeds {
public:
    bool begin() override {
        return true;
    }

    bool brightness(uint8_t value, bool refresh = true) override {
        return true;
    }

    void off() override {
    }

    void on() override {
    }

    void color(uint8_t position, Color color, bool refresh = true) override {
    }

    void off(uint8_t position) override {
    }

    void on(uint8_t position) override {
    }

    bool refresh() override {
        return true;
    }
};

NoopModuleLeds noop_leds;
#if defined(FK_HARDWARE_FULL)
MetalLeds module_leds;
#else
LinuxLeds module_leds;
#endif

ModuleLeds *get_module_leds() {
    return &module_leds;
}

} // namespace fk
