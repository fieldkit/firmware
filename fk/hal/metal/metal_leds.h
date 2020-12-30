#pragma once

#include "hal/hal.h"

namespace fk {

class MetalLeds : public ModuleLeds {
private:
    Availability status_{ Availability::Unknown };
    static constexpr size_t NumberOfPixels = 4;
    uint8_t pixels_[1][1 + NumberOfPixels + NumberOfPixels * 3];
    uint8_t active_{ 0 };
    bool dirty_{ false };

public:
    MetalLeds();

public:
    bool begin() override;
    bool brightness(uint8_t value, bool refresh = true) override;
    void off() override;
    void on() override;
    void color(uint8_t position, Color color, bool refresh = true) override;
    void off(uint8_t position) override;
    void on(uint8_t position) override;
    bool refresh() override;

};

}
