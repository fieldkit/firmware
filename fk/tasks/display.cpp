#include "hal/metal/metal.h"
#include "pool.h"
#include "state_manager.h"
#include "storage/storage.h"
#include "tasks/tasks.h"
#include "timer.h"

#include "display/build_view.h"
#include "display/display_views.h"
#include "display/gps_view.h"
#include "display/home_view.h"
#include "display/leds.h"
#include "display/lora_view.h"
#include "display/menu_view.h"
#include "display/message_view.h"
#include "display/module_status_view.h"
#include "display/name_view.h"
#include "display/qr_code_view.h"
#include "display/readings_view.h"
#include "display/schedule_view.h"
#include "display/self_check_view.h"
#include "display/debug_module_view.h"

namespace fk {

FK_DECLARE_LOGGER("display");

class MainViewController : public ViewController {
private:
    Pool *pool_{ nullptr };
    HomeView home_view;
    ReadingsView readings_view;
    MenuView menu_view;
    NameView name_view;
    ScheduleView schedule_view;
    BuildView build_view;
    MessageView message_view;
    SelfCheckView self_check_view;
    ModuleStatusView module_status_view;
    QrCodeView qr_code_view;
    LoraView lora_view;
    GpsView gps_view;
    LedsController leds;
    FaultView fault_view;
    DisplayView *view = &home_view;
    uint32_t notified_{ 0 };
    uint32_t updated_{ 0 };

public:
    explicit MainViewController(Pool &pool) : pool_(&pool), menu_view{ this, pool } {
        instance_ = this;
    }

    virtual ~MainViewController() {
        instance_ = nullptr;
    }

private:
    void show_view(DisplayView &new_view) {
        view->hide();
        view = &new_view;
        view->show();
    }

public:
    void show_menu() override {
        show_view(menu_view);
    }

    void show_home() override {
        show_view(home_view);
    }

    void show_self_check() override {
        show_view(self_check_view);
    }

    void show_readings() override {
        show_view(readings_view);
    }

    void show_schedule(ScheduleType type) override {
        schedule_view.type(type);
        show_view(schedule_view);
    }

    void show_name() override {
        home_view.show_name();
        show_view(name_view);
    }

    void show_build() override {
        home_view.show_build();
        show_view(home_view);
    }

    void show_module_status() override {
        show_view(module_status_view);
    }

    void show_module_menu(uint8_t bay) override {
        menu_view.show_for_module(bay);
        show_view(menu_view);
    }

    void show_message(const char *message, uint32_t visible_ms) override {
        message_view.message(message, visible_ms);
        show_view(message_view);
        StandardPool pool{ "display-frame" };
        view->tick(this, pool);
    }

    void show_qr_code() override {
        show_view(qr_code_view);
    }

    void show_lora() override {
        show_view(lora_view);
    }

    void show_gps() override {
        show_view(gps_view);
    }

    void show_fault(FaultCode *code) {
        fault_view.fault_code(code);
        show_view(fault_view);
    }

    void on_external() override {
        show_home(); // show_qr_code
#if !defined(FK_DISABLE_NETWORK)
        get_ipc()->launch_worker(create_pool_worker<WifiToggleWorker>(WifiToggleWorker::DesiredState::Enabled));
#endif
    }

    void refresh_notifications() {
        auto gs = get_global_state_ro();
        auto &notification = gs.get()->notification;
        if (notification.created > 0 && notified_ < notification.created) {
            loginfo("notif: '%s'", notification.message);
            notified_ = notification.created;
            show_message(notification.message, 0);
        }

        auto &display = gs.get()->display;
        if (display.open_menu.time > 0 && display.open_menu.time > updated_) {
            loginfo("open-menu");
            updated_ = display.open_menu.time;
            if (display.open_menu.readings) {
                show_view(menu_view);
                menu_view.show_readings();
            }
        }
    }

    void run(display_params_t *params) {
        if (!leds.begin()) {
            logwarn("leds unavailable");
        }

        refresh_notifications();

        IntervalTimer stop_timer{ FiveMinutesMs };
        IntervalTimer notifications_timer{ OneSecondMs / 10 };
        auto maximum_used = 0u;
        auto frame_pool = pool_->subpool("display-frame", 1024);
        auto can_stop = os_task_is_running(&scheduler_task);
        auto should_show_readings = params->readings;
        auto dequeue_button = false;

        FaultCode *incoming_fault_code = nullptr;
        while (!can_stop || !stop_timer.expired()) {
            if (!view->custom_leds()) {
                leds.tick();
            }

            if (notifications_timer.expired()) {
                refresh_notifications();
            }

            if (dequeue_button) {
                Button *button = nullptr;
                if (get_ipc()->dequeue_button(&button)) {
                    stop_timer.mark();

                    switch (button->index()) {
                    case Buttons::Right: {
                        loginfo("down");
                        view->down(this);
                        break;
                    }
                    case Buttons::Middle: {
                        loginfo("enter");
                        view->enter(this);
                        break;
                    }
                    case Buttons::Left: {
                        loginfo("up");
                        view->up(this);
                        break;
                    }
                    case Buttons::External: {
                        loginfo("external");
                        view->external(this);
                        break;
                    }
                    default: {
                        break;
                    }
                    }
                }
            } else {
                dequeue_button = true;
            }

            if (frame_pool->used() > 0) {
                if (frame_pool->used() > maximum_used) {
                    maximum_used = frame_pool->used();
                    loginfo("new display usage maximum: %zu", maximum_used);
                }
                frame_pool->clear();
            }

            auto fault = fk_fault_get();
            if (incoming_fault_code != fault) {
                incoming_fault_code = fault;
                if (incoming_fault_code != nullptr) {
                    loginfo("show fault");
                    show_fault(incoming_fault_code);
                }
            }

            if (should_show_readings) {
                show_readings();
                should_show_readings = false;
            }

            view->tick(this, *frame_pool);
        }

        view->hide();
    }
};

void task_handler_display(void *params) {
    StandardPool pool{ "display" };
    MainViewController views{ pool };
    views.run((display_params_t *)params);
    get_module_leds()->off();
    get_display()->off();
}

} // namespace fk
