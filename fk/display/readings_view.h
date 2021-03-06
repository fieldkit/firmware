#pragma once

#include "common.h"
#include "display_views.h"
#include "state_ref.h"

namespace fk {

class ReadingsView : public DisplayView {
private:
    uint32_t dirty_{ true };
    uint32_t position_{ 0 };

public:
    ReadingsView() { }

public:
    void tick(ViewController *views, Pool &pool) override;
    void up(ViewController *views) override;
    void down(ViewController *views) override;
    void enter(ViewController *views) override;

};

}
