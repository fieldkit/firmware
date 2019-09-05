#pragma once

#include "common.h"

namespace fk {

typedef struct xbm_data_t {
    uint8_t w;
    uint8_t h;
    const uint8_t *data;
} xbm_data_t;

struct DisplayScreen {
};

struct TaskProgress {
    const char *operation;
    float progress;
};

struct HomeScreen : public DisplayScreen {
    uint32_t time;
    bool network;
    bool gps;
    bool recording;
    bool logo;
    float battery;
    const char *message;
    TaskProgress progress;
};

struct MenuOption {
    const char *label;
    bool selected;
    bool visible;

    MenuOption(const char *label) : label(label), selected(false) {
    }

    virtual void on_selected() = 0;
};

struct SimpleScreen : public DisplayScreen {
    const char *message;

    SimpleScreen() {
    }

    SimpleScreen(const char *message) : message(message) {
    }
};

template<typename T>
struct LambdaOption : public MenuOption {
    T fn;

    LambdaOption(const char *label, T fn) : MenuOption(label), fn(fn) {
    }

    void on_selected() override {
        fn();
    }
};

template<typename T>
LambdaOption<T> to_lambda_option(const char *label, T fn) {
    return LambdaOption<T>(label, fn);
}

struct MenuScreen : public DisplayScreen {
    /**
     * A NULL value indicates the end of this array.
     */
    MenuOption **options{ nullptr };

    MenuScreen();
    MenuScreen(MenuOption **options);
};

struct QrCodeScreen : public DisplayScreen {
    size_t size;
    uint8_t *data;
};

struct Check {
    const char *name;
    bool pass;
};

struct SelfCheckScreen : public DisplayScreen {
    Check **checks;
};

class Display {
public:
    virtual void on() = 0;
    virtual void off() = 0;
    virtual void centered(const xbm_data_t &xbm) = 0;
    virtual void company_logo() = 0;
    virtual void fk_logo() = 0;
    virtual void home(HomeScreen const &screen) = 0;
    virtual void menu(MenuScreen const &screen) = 0;
    virtual void qr(QrCodeScreen const &screen) = 0;
    virtual void self_check(SelfCheckScreen const &screen) = 0;
    virtual void simple(SimpleScreen &screen) = 0;

};

class NullDisplay : public Display {
public:
    void on() override { }
    void off() override { }
    void centered(const xbm_data_t &xbm) override { }
    void company_logo() override { }
    void fk_logo() override { }
    void home(HomeScreen const &screen) override { }
    void menu(MenuScreen const &screen) override { }
    void qr(QrCodeScreen const &screen) override { }
    void self_check(SelfCheckScreen const &screen) override { }
    void simple(SimpleScreen &screen) override { }

    bool begin() {
        return true;
    }

};

Display *get_display();

}
