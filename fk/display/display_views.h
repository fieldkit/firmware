#pragma once

#include "common.h"
#include "pool.h"

namespace fk {

class ViewController {
protected:
    static ViewController *instance_;

public:
    static ViewController *get() {
        return instance_;
    }

public:
    virtual void show_menu() = 0;
    virtual void show_home() = 0;
    virtual void show_self_check() = 0;
    virtual void show_readings() = 0;
    virtual void show_name() = 0;
    virtual void show_build() = 0;
    virtual void show_module_status() = 0;
    virtual void show_module_menu(uint8_t bay) = 0;
    virtual void show_message(const char *message, uint32_t visible_ms = 0) = 0;
    virtual void show_qr_code() = 0;
    virtual void show_lora() = 0;
    virtual void show_gps() = 0;
    virtual void show_schedule() = 0;
    virtual void on_external() = 0;
};

class DisplayView {
public:
    virtual void tick(ViewController *views, Pool &pool) = 0;

public:
    virtual void hide() {
    }

    virtual void show() {
    }

    virtual void external(ViewController *views) {
        views->on_external();
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

    virtual bool custom_leds() {
        return false;
    }
};

} // namespace fk
