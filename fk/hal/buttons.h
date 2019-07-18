#pragma once

#include "common.h"

namespace fk {

class Button {
private:
    const char *name_;
    bool down_{ false };
    uint32_t time_{ 0 };

public:
    Button(const char *name);

public:
    void changed(uint8_t pin, bool down);

};

class Buttons {
protected:
    Button buttons_[3]{
        { "left" },
        { "middle" },
        { "right" },
    };

public:
    virtual bool begin() = 0;

};

Buttons *get_buttons();

}
