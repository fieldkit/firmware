#include <tiny_printf.h>

#include "hal/board.h"
#include "hal/display.h"
#include "hal/ipc.h"
#include "platform.h"
#include "readings_view.h"
#include "state_ref.h"

namespace fk {

FK_DECLARE_LOGGER("readings");

static void add_reading(collection<DisplayReading> &readings, state::AttachedModule *am, state::AttachedSensor *sensor, Pool &pool) {
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
    views->show_home();
}

struct SensorReadingsConfig {
    bool voltages{ false };
};

struct SensorReadingOption : public MenuOption {
    SensorReadingsConfig *config_;
    uint32_t sensor_index_;
    char reading_[32];
    char help_[32];
    bool show_help_{ false };

    SensorReadingOption(SensorReadingsConfig *config, uint32_t sensor_index)
        : MenuOption("..."), config_(config), sensor_index_(sensor_index) {
    }

    void on_selected() override {
        show_help_ = !show_help_;
        if (show_help_) {
            label_ = help_;
        } else {
            label_ = reading_;
        }
    }

    void refresh(GlobalState const *gs) override {
        auto attached = gs->dynamic.attached();
        if (attached != nullptr) {
            auto mas = attached->get_nth_sensor(sensor_index_);
            auto reading = mas.sensor->reading();
            auto position = mas.attached_module->position().integer();
            if (reading.calibrated.has_value()) {
                logverbose("[%d] refresh: %s %.3f", sensor_index_, mas.sensor->name(), reading.calibrated.value());
            } else {
                logverbose("[%d] refresh: %s <none>", sensor_index_, mas.sensor->name());
            }
            auto value = config_->voltages ? reading.uncalibrated : reading.calibrated;
            auto suffix = config_->voltages ? mas.sensor->uncalibrated_unit_of_measure() : mas.sensor->unit_of_measure();
            if (position == ModulePosition::Virtual.integer()) {
                if (value.has_value()) {
                    tiny_snprintf(reading_, sizeof(reading_), "[%c] %.3f%s", ' ', value.value(), suffix);
                } else {
                    tiny_snprintf(reading_, sizeof(reading_), "[%c] <none>", ' ');
                }
            } else {
                if (value.has_value()) {
                    tiny_snprintf(reading_, sizeof(reading_), "[%d] %.3f%s", position, value.value(), suffix);
                } else {
                    tiny_snprintf(reading_, sizeof(reading_), "[%d] <none>", position);
                }
            }
            tiny_snprintf(help_, sizeof(help_), "[%c] %s", ' ', mas.sensor->name());
            if (label_ == nullptr || (label_ != reading_ && label_ != help_)) {
                label_ = reading_;
            }
        }
    }
};

SensorReadingOption *to_sensor_reading_option(Pool *pool, SensorReadingsConfig *config, uint32_t sensor_index) {
    return new (*pool) SensorReadingOption(config, sensor_index);
}

MenuScreen *create_readings_menu(GlobalState const *gs, MenuOption *back_option, Pool &pool) {
    auto attached = gs->dynamic.attached();
    if (attached == nullptr) {
        return nullptr;
    }

    auto nsensors = attached->number_of_sensors();
    auto config = new (pool) SensorReadingsConfig();
    auto noptions = (nsensors + 3);
    auto options = (MenuOption **)pool.malloc(sizeof(MenuOption *) * noptions);
    auto sensor_index = 0u;
    auto option_index = 0u;

    for (auto &attached_module : attached->modules()) {
        if (true || !attached_module.is_virtual()) {
            for (auto &sensor : attached_module.sensors()) {
                FK_ASSERT(option_index < nsensors);
                auto option = to_sensor_reading_option(&pool, config, sensor_index);
                option->refresh(gs);
                options[option_index] = option;
                sensor_index++;
                option_index++;
                (void)sensor;
            }
        } else {
            sensor_index += attached_module.sensors().size();
        }
    }

    options[option_index++] = to_lambda_option(&pool, "V/U", [=]() {
        config->voltages = !config->voltages;
        for (auto i = 0u; i < noptions - 1; i++) {
            if (options[i] != nullptr) {
                options[i]->refresh(gs);
            }
        }
    });

    auto back_that_stops_polling = to_lambda_option(&pool, "Back", [=]() {
        get_ipc()->signal_workers(WorkerCategory::Polling, 9);
        back_option->on_selected();
    });

    options[option_index++] = back_that_stops_polling;

    options[option_index] = nullptr;

    return new (pool) MenuScreen("readings", options);
}

} // namespace fk
