#include <tiny_printf.h>

#include "hal/board.h"
#include "hal/display.h"
#include "platform.h"
#include "readings_view.h"
#include "state_ref.h"

namespace fk {

FK_DECLARE_LOGGER("readings");

static void add_reading(collection<DisplayReading> &readings, state::AttachedModule *am, state::AttachedSensor *sensor,
                        Pool &pool) {
    auto module_wo_modules = am->name();
    if (strstr(module_wo_modules, "modules.") == module_wo_modules) {
        module_wo_modules += strlen("modules.");
    }
    auto position = am->position().integer();
    auto pretty_module_line = pool.sprintf("%d: %s", position, module_wo_modules);
    auto reading = sensor->reading();

    readings.emplace(pretty_module_line, sensor->name(), reading.calibrated);
}

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
    if (skip_diagnostics_) {
        skip_diagnostics_ = false;
        for (auto index = 0u; index < number_sensors; index++) {
            auto mas = attached->get_nth_sensor(index);
            if (mas.attached_module->position().integer() != 255) {
                position_ = index;
                break;
            }
        }
    }

    auto index = position_ % number_sensors;
    auto mas = attached->get_nth_sensor(index);
    if (mas.sensor != nullptr) {
        collection<DisplayReading> readings{ pool };
        add_reading(readings, mas.attached_module, mas.sensor, pool);
        ReadingScreen reading_screen{ &readings };
        auto bus = get_board()->i2c_core();
        auto display = get_display();
        display->reading(reading_screen);
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
    logwarn("show home");
    views->show_home();
}

template <typename TSelect> struct SensorReadingOption : public MenuOption {
    uint32_t sensor_index_;
    TSelect select_fn_;
    char buffer_[32];

    SensorReadingOption(uint32_t sensor_index, TSelect select_fn)
        : MenuOption("..."), sensor_index_(sensor_index), select_fn_(select_fn) {
    }

    void on_selected() override {
        select_fn_();
    }

    void refresh(GlobalState const *gs) override {
        auto attached = gs->dynamic.attached();
        if (attached != nullptr) {
            auto mas = attached->get_nth_sensor(sensor_index_);
            auto reading = mas.sensor->reading();
            auto position = mas.attached_module->position().integer();
            logverbose("[%d] refresh: %s %f", sensor_index_, mas.sensor->name(), reading.calibrated);
            tiny_snprintf(buffer_, sizeof(buffer_), "[%d] %f", position, reading.calibrated);
            label_ = buffer_;
        }
    }
};

template <typename TSelect>
SensorReadingOption<TSelect> *to_sensor_reading_option(Pool *pool, uint32_t sensor_index, TSelect fn) {
    return new (*pool) SensorReadingOption<TSelect>(sensor_index, fn);
}

MenuScreen *create_readings_menu(GlobalState const *gs, MenuOption *back_option, Pool &pool) {
    auto attached = gs->dynamic.attached();
    if (attached == nullptr) {
        return nullptr;
    }

    auto nsensors = attached->number_of_physical_sensors();
    auto options = (MenuOption **)pool.malloc(sizeof(MenuOption *) * (nsensors + 2));
    auto sensor_index = 0u;
    auto option_index = 0u;

    for (auto &attached_module : attached->modules()) {
        if (!attached_module.is_virtual()) {
            for (auto &sensor : attached_module.sensors()) {
                FK_ASSERT(option_index < nsensors);
                auto option = to_sensor_reading_option(&pool, sensor_index, [=]() { (void)sensor; });
                option->refresh(gs);
                options[option_index] = option;
                sensor_index++;
                option_index++;
            }
        } else {
            sensor_index += attached_module.sensors().size();
        }
    }

    back_option->focused(false);

    options[option_index++] = back_option;

    options[option_index] = nullptr;

    return new (pool) MenuScreen("readings", options);
}

} // namespace fk
