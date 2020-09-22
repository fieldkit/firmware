#pragma once

#include "common.h"
#include "display_views.h"
#include "hal/modmux.h"

namespace fk {

class ModuleStatusView : public DisplayView {
private:
    ModMux::iterator module_;

public:
    ModuleStatusView();

public:
    void tick(ViewController *views, Pool &pool) override;
    void up(ViewController *views) override;
    void down(ViewController *views) override;
    void enter(ViewController *views) override;

};

}
