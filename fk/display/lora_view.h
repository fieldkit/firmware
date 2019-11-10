#pragma once

#include "common.h"
#include "hal/display.h"
#include "display_views.h"

namespace fk {

class LoraView : public DisplayView {
private:
    char status_[64];

public:
    void tick(ViewController *views) override;

};

}
