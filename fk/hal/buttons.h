#pragma once

#include "common.h"
#include "activity.h"

namespace fk {

class Button : public Activity {
private:
    const char *name_;
    uint8_t index_;
    uint32_t debounce_{ 0 };
    uint32_t time_{ 0 };
    uint32_t pressed_{ 0 };
    bool down_{ false };
    bool had_display_{ false };

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

private:
    bool is_up() const;
    bool is_enter() const;
    bool is_down() const;
    bool is_external() const;
};

class Buttons {
public:
    constexpr static uint8_t NumberOfButtons = 4;
    constexpr static uint8_t Left = 0;
    constexpr static uint8_t Middle = 1;
    constexpr static uint8_t Right = 2;
    constexpr static uint8_t External = 3;

protected:
    Button buttons_[NumberOfButtons]{ { "L", Left }, { "M", Middle }, { "R", Right }, { "E", External } };

public:
    virtual bool begin() = 0;

    virtual bool is_debug_pressed() const = 0;

    virtual bool get(uint8_t which) const = 0;
};

Buttons *get_buttons();

} // namespace fk
