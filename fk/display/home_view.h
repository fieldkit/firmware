#pragma once

#include "common.h"
#include "hal/display.h"
#include "display_views.h"

namespace fk {

class HomeView : public DisplayView {
public:
    void tick(ViewController *views) override;
    void up(ViewController *views) override;
    void down(ViewController *views) override;
    void enter(ViewController *views) override;

};

}
