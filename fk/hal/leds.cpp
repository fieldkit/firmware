#include "hal/leds.h"
#include "hal/metal/metal.h"
#include "hal/linux/linux.h"

namespace fk {

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

#if defined(FK_HARDWARE_FULL)
#if defined(FK_UNDERWATER)
NoopModuleLeds module_leds;
#else
MetalLeds module_leds;
#endif
#else
LinuxLeds module_leds;
#endif

ModuleLeds *get_module_leds() {
    return &module_leds;
}

} // namespace fk
