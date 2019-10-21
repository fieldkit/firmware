#pragma once

#include "common.h"
#include "hal/display.h"
#include "display_views.h"
#include "config.h"

namespace fk {

class HomeView : public DisplayView {
public:
    enum class Visible {
        Name,
        IP,
        Build,
        Uptime,
    };

private:
    char primary_[64];
    char secondary_[64];
    uint8_t visible_{ 0 };

public:
    void tick(ViewController *views) override;
    void up(ViewController *views) override;
    void down(ViewController *views) override;
    void enter(ViewController *views) override;

public:
    void show_name();
    void show_build();
    void show_uptime();

};

}
