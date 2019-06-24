#pragma once

#include "common.h"

namespace fk {

typedef struct xbm_data_t {
    uint8_t w;
    uint8_t h;
    uint8_t *data;
} xbm_data_t;

typedef struct home_screen_t {
    uint32_t time;
    bool wifi;
    bool gps;
    float battery;
} home_screen_t;

class Display {
public:
    virtual void on();
    virtual void off();
    virtual void centered(xbm_data_t &xbm);
    virtual void company_logo();
    virtual void fk_logo();
    virtual void home(home_screen_t data);

};

class NullDisplay : public Display {
public:
    void on() override { }
    void off() override { }
    void centered(xbm_data_t &xbm) override { }
    void company_logo() override { }
    void fk_logo() override { }
    void home(home_screen_t data) override { }

};

class DisplayFactory {
public:
    Display *get_display();

};

}
