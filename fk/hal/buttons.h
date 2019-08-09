#pragma once

#include "common.h"

namespace fk {

class Activity {
};

class Button : public Activity {
private:
    const char *name_;
    uint8_t index_;
    uint32_t time_{ 0 };
    uint32_t pressed_{ 0 };
    bool down_{ false };

public:
    Button(const char *name, uint8_t index);

public:
    void changed(bool down);

    uint32_t pressed() const {
        return pressed_;
    }

    bool down() const {
        return down_;
    }

    uint8_t index() const {
        return index_;
    }

    bool is_up() const;
    bool is_enter() const;
    bool is_down() const;

};

class Buttons {
public:
    constexpr static uint8_t NumberOfButtons = 3;
    constexpr static uint8_t Left = 0;
    constexpr static uint8_t Middle = 1;
    constexpr static uint8_t Right = 2;

protected:
    Button buttons_[NumberOfButtons]{ { "L", Left }, { "M", Middle }, { "R", Right } };

public:
    virtual bool begin() = 0;

    virtual uint8_t number_pressed() const = 0;

};

Buttons *get_buttons();

}
