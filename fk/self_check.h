#pragma once

#include "common.h"
#include "display.h"

namespace fk {

class SelfCheck {
private:
    Display *display_;

public:
    SelfCheck(Display *display);

public:
    void check();

};

}
