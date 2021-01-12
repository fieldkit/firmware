#pragma once

#include "common.h"
#include "pool.h"
#include "config.h"

namespace fk {

class GlobalState;

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

struct WorkerInfo {
    const char *name;
    float progress;
    bool visible;
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

    struct PowerInfo {
        float battery;
    };

    uint32_t time;
    Network network;
    Gps gps;
    bool recording;
    bool logo;
    const char *debug_mode;
    PowerInfo power;
    const char *primary;
    const char *secondary;
    TaskProgress progress;
    WorkerInfo workers[NumberOfWorkerTasks];
    uint32_t readings;
};

struct MenuOption {
    const char *label_;
    bool focused_;
    bool selected_;
    bool visible_;
    bool active_;

    MenuOption(const char *label) : label_(label), focused_(false), selected_(false), visible_(true), active_(true) {
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

    virtual bool focused() const {
        return focused_;
    }

    virtual void focused(bool value) {
        focused_ = value;
    }

    virtual const char *label() const {
        return label_;
    }

    virtual void refresh(GlobalState const *gs) {
    }
};

struct SimpleScreen : public DisplayScreen {
    const char *message{ nullptr };
    const char *secondary{ nullptr };

    explicit SimpleScreen() {
    }

    explicit SimpleScreen(const char *message) : message(message) {
    }

    explicit SimpleScreen(const char *message, const char *secondary) : message(message), secondary(secondary) {
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

template<typename TSelect>
struct LambdaOption : public MenuOption {
    TSelect select_fn;

    LambdaOption(const char *label, TSelect select_fn) : MenuOption(label), select_fn(select_fn) {
    }

    void on_selected() override {
        select_fn();
    }
};

template<typename TSelect, typename TSelected>
struct SelectableLambdaOption : public MenuOption {
    TSelect select_fn;
    TSelected selected_fn;

    SelectableLambdaOption(const char *label, TSelect select_fn, TSelected selected_fn) : MenuOption(label), select_fn(select_fn), selected_fn(selected_fn) {
    }

    void on_selected() override {
        select_fn();
    }

    void refresh(GlobalState const *gs) override {
        selected(selected_fn(gs));
    }
};

struct MenuScreen : public DisplayScreen {
    const char *title{ nullptr };

    /**
     * A NULL value indicates the end of this array.
     */
    MenuOption **options{ nullptr };

    MenuScreen();
    MenuScreen(const char *title, MenuOption **options);

    void refresh(GlobalState const *gs) {
        FK_ASSERT(options != nullptr);
        FK_ASSERT_ADDRESS(options);
        for (auto i = 0u; options[i] != nullptr; ++i) {
            options[i]->refresh(gs);
        }
    }
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
    const char *name;
    const char *message;

    ModuleStatusScreen() {
    }

    ModuleStatusScreen(uint8_t bay, const char *name, const char *message) : bay(bay), name(name), message(message) {
    }
};

class Display {
public:
    virtual bool begin() = 0;
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
    bool begin() override { return true; }
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

};

Display *get_display();

}
