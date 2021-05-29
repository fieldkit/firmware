#include "readings_view.h"
#include "state_ref.h"
#include "hal/board.h"
#include "hal/display.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("readings");

void ReadingsView::tick(ViewController *views, Pool &pool) {
    if (fk_uptime() < dirty_) {
        return;
    }

    dirty_ = fk_uptime() + 1000;

    auto gs = get_global_state_ro();

    auto attached = gs.get()->dynamic.attached();

    if (attached == nullptr) {
        return;
    }

    auto number_sensors = attached->number_of_sensors();

    auto index = position_ % number_sensors;
    auto mas = attached->get_nth_sensor(index);
    if (mas.sensor != nullptr) {
        auto reading = mas.sensor->reading();

        ReadingScreen reading_screen{ mas.attached_module->name(), mas.sensor->name(), reading.calibrated };
        auto bus = get_board()->i2c_core();
        auto display = get_display();
        display->reading(reading_screen);
        return;
    }
}

void ReadingsView::up(ViewController *views) {
    position_--;
    dirty_ = 0;
}

void ReadingsView::down(ViewController *views) {
    position_++;
    dirty_ = 0;
}

void ReadingsView::enter(ViewController *views) {
    views->show_home();
}

}
