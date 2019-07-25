#pragma once

#include "common.h"

namespace fk {

typedef struct xbm_data_t {
    uint8_t w;
    uint8_t h;
    const uint8_t *data;
} xbm_data_t;

typedef struct home_screen_t {
    uint32_t time;
    bool wifi;
    bool gps;
    float battery;
} home_screen_t;

class Display {
public:
    virtual void on() = 0;
    virtual void off() = 0;
    virtual void centered(const xbm_data_t &xbm) = 0;
    virtual void company_logo() = 0;
    virtual void fk_logo() = 0;
    virtual void home(home_screen_t data) = 0;

};

class NullDisplay : public Display {
public:
    void on() override { }
    void off() override { }
    void centered(const xbm_data_t &xbm) override { }
    void company_logo() override { }
    void fk_logo() override { }
    void home(home_screen_t data) override { }

    bool begin() {
        return true;
    }

};

Display *get_display();

}
