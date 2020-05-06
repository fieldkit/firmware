#pragma once

#include "common.h"
#include "hal/display.h"
#include "display_views.h"

namespace fk {

class GpsView : public DisplayView {
public:
    void tick(ViewController *views, Pool &pool) override;
    void hide() override;

};

}
