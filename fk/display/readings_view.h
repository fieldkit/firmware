#pragma once

#include "common.h"
#include "display_views.h"
#include "state_ref.h"

namespace fk {

class ReadingsView : public DisplayView {
public:
    void tick(ViewController *views) override;

};

}
