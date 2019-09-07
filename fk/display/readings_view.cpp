#include "readings_view.h"
#include "state_ref.h"
#include "hal/board.h"
#include "hal/display.h"

namespace fk {

void ReadingsView::tick(ViewController *views) {
    SimpleScreen simple = { "Readings" };

    auto bus = get_board()->i2c_core();
    auto display = get_display();
    display->simple(simple);
}

}
