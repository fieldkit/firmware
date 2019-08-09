#pragma once

#include "common.h"

namespace fk {

class Activity {
};

class Button : public Activity {
private:
    const char *name_;
    uint32_t time_{ 0 };
    bool down_{ false };

public:
    Button(const char *name);

public:
    void changed(bool down);

    bool down() const {
        return down_;
    }

};

class Buttons {
public:
    constexpr static uint8_t NumberOfButtons = 3;
    constexpr static uint8_t Left = 0;
    constexpr static uint8_t Middle = 1;
    constexpr static uint8_t Right = 2;

protected:
    Button buttons_[NumberOfButtons]{ { "L" }, { "M" }, { "R" } };

public:
    virtual bool begin() = 0;

    virtual uint8_t number_pressed() const = 0;

};

Buttons *get_buttons();

}
