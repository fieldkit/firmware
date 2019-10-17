#pragma once

#include "common.h"
#include "display_views.h"

namespace fk {

class ModuleStatusView : public DisplayView {
private:
    uint8_t bay_{ 0 };

public:
    void tick(ViewController *views) override;
    void up(ViewController *views) override;
    void down(ViewController *views) override;
    void enter(ViewController *views) override;

};

}
