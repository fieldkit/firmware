#pragma once

#include "common.h"
#include "pool.h"

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
    struct Gps {
        bool enabled;
        bool fix;
    };

    struct Network {
        bool enabled;
        bool connected;
        uint32_t bytes_rx;
        uint32_t bytes_tx;
    };

    uint32_t time;
    Network network;
    Gps gps;
    bool recording;
    bool logo;
    bool debug_mode;
    float battery;
    const char *primary;
    const char *secondary;
    TaskProgress progress;
};

struct MenuOption {
    const char *label_;
    bool selected_;
    bool visible_;
    bool active_;

    MenuOption(const char *label) : label_(label), selected_(false), visible_(true), active_(true) {
    }

    virtual void on_selected() = 0;

    virtual bool active() const {
        return active_;
    }
    virtual void active(bool active) {
        active_ = active;
    }

    virtual bool visible() const {
        return visible_;
    }
    virtual void visible(bool visible) {
        visible_ = visible;
    }

    virtual bool selected() const {
        return selected_;
    }
    virtual void selected(bool value) {
        selected_ = value;
    }

    virtual const char *label() const {
        return label_;
    }
};

struct SimpleScreen : public DisplayScreen {
    const char *message;
    const char *secondary;

    SimpleScreen() {
    }

    SimpleScreen(const char *message) : message(message) {
    }

    SimpleScreen(const char *message, const char *secondary) : message(message), secondary(secondary) {
    }
};

struct ReadingScreen : public DisplayScreen {
    const char *module;
    const char *sensor;
    float value;

    ReadingScreen() {
    }

    ReadingScreen(const char *module, const char *sensor, float value) : module(module), sensor(sensor), value(value) {
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

struct ModuleStatusScreen : public DisplayScreen {
    uint8_t bay;
    const char *message;

    ModuleStatusScreen() {
    }

    ModuleStatusScreen(uint8_t bay, const char *message) : bay(bay), message(message) {
    }
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
    virtual void simple(SimpleScreen &&screen) = 0;
    virtual void reading(ReadingScreen &screen) = 0;
    virtual void module_status(ModuleStatusScreen &screen) = 0;

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
    void simple(SimpleScreen &&screen) override { }
    void reading(ReadingScreen &screen) override { }
    void module_status(ModuleStatusScreen &screen) override { }

    bool begin() {
        return true;
    }

};

Display *get_display();

}
