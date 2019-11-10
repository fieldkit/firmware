#include "tasks/tasks.h"
#include "hal/metal/metal.h"
#include "pool.h"
#include "state_manager.h"
#include "storage/storage.h"

#include "display/display_views.h"
#include "display/home_view.h"
#include "display/name_view.h"
#include "display/build_view.h"
#include "display/qr_code_view.h"
#include "display/readings_view.h"
#include "display/self_check_view.h"
#include "display/menu_view.h"
#include "display/message_view.h"
#include "display/module_status_view.h"
#include "display/lora_view.h"

namespace fk {

FK_DECLARE_LOGGER("display");

class MainViewController : public ViewController {
private:
    HomeView home_view;
    ReadingsView readings_view;
    MenuView menu_view;
    NameView name_view;
    BuildView build_view;
    MessageView message_view;
    SelfCheckView self_check_view;
    ModuleStatusView module_status_view;
    QrCodeView qr_code_view;
    LoraView lora_view;
    DisplayView *view = &home_view;

public:
    MainViewController(Pool &pool) : menu_view{ this, pool } {
    }

public:
    void show_menu() override {
        view = &menu_view;
        view->show();
    }

    void show_home() override {
        view = &home_view;
        view->show();
    }

    void show_self_check() override {
        view = &self_check_view;
        view->show();
    }

    void show_readings() override {
        view = &readings_view;
        view->show();
    }

    void show_name() override {
        home_view.show_name();
        view = &name_view;
        view->show();
    }

    void show_build() override {
        home_view.show_build();
        view = &home_view;
        view->show();
    }

    void show_module_status() override {
        view = &module_status_view;
        view->show();
    }

    void show_message(const char *message) override {
        message_view.message(message);
        view = &message_view;
        view->show();
        view->tick(this);
    }

    void show_qr_code() override {
        view = &qr_code_view;
        view->show();
    }

    void show_lora() override {
        view = &lora_view;
        view->show();
    }

    void on_external() override {
        show_qr_code();

        auto worker = create_pool_worker<WifiToggleWorker>(WifiToggleWorker::DesiredState::Enabled);
        get_ipc()->launch_worker(worker);
    }

    void run() {
        auto stop_time = fk_uptime() + fk_config().display.inactivity;
        auto can_stop = os_task_is_running(&scheduler_task);

        while (!can_stop || fk_uptime() < stop_time) {
            view->tick(this);

            Button *button = nullptr;
            if (get_ipc()->dequeue_button(&button)) {
                stop_time = fk_uptime() + fk_config().display.inactivity;

                switch (button->index()) {
                case Buttons::Left: {
                    loginfo("down");
                    view->down(this);
                    break;
                }
                case Buttons::Middle: {
                    loginfo("enter");
                    view->enter(this);
                    break;
                }
                case Buttons::Right: {
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
        }
    }

};

void task_handler_display(void *params) {
    MallocPool pool{ "display", DefaultWorkerPoolSize };
    MainViewController views{ pool };
    views.run();
    get_display()->off();
}

}
