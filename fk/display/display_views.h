#pragma once

namespace fk {

class ViewController {
public:
    virtual void show_menu() = 0;
    virtual void show_home() = 0;
    virtual void show_self_check() = 0;
    virtual void show_readings() = 0;
    virtual void show_name() = 0;
    virtual void show_build() = 0;

};

class DisplayView {
public:
    virtual void tick(ViewController *views) = 0;

public:
    virtual void show() {
    }

    virtual void up(ViewController *views) {
        views->show_home();
    }

    virtual void down(ViewController *views) {
        views->show_home();
    }

    virtual void enter(ViewController *views) {
        views->show_home();
    }

};

}
