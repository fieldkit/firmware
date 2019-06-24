#pragma once

namespace fk {

typedef struct xbm_data_t {
    uint8_t w;
    uint8_t h;
    uint8_t *data;
} xbm_data_t;

class Display {
public:
    virtual void on();
    virtual void off();
    virtual void company_logo();
    virtual void fk_logo();
    virtual void centered(xbm_data_t &xbm);

};

class NullDisplay : public Display {
public:
    void on() override { }
    void off() override { }
    void centered(xbm_data_t &xbm) override { }
    void company_logo() override { }
    void fk_logo() override { }

};

class DisplayFactory {
public:
    Display *get_display();

};

}
