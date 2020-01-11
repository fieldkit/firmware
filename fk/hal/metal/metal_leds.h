#pragma once

#include "hal/hal.h"

namespace fk {

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class MetalLeds : public ModuleLeds {
private:
    static constexpr size_t NumberOfPixels = 4;
    uint8_t pixels_[1 + NumberOfPixels + NumberOfPixels * 3];

public:
    MetalLeds();

public:
    bool begin() override;
    void brightness(uint8_t value) override;
    void off() override;
    void on() override;
    void color(uint8_t position, Color color);
    void off(uint8_t position);
    void on(uint8_t position);

private:
    bool refresh();

};

}
