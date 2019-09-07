#pragma once

#include "common.h"
#include "hal/display.h"
#include "display_views.h"
#include "display_self_check_callbacks.h"
#include "simple_workers.h"

namespace fk {

class SelfCheckView : public DisplayView {
private: 
    DisplaySelfCheckCallbacks self_check_callbacks_;

public:
    void show() override {
        self_check_callbacks_.clear();
        auto worker = create_pool_wrapper<SelfCheckWorker, DefaultWorkerPoolSize, PoolWorker<SelfCheckWorker>>(self_check_callbacks_);
        if (!get_ipc()->launch_worker(worker)) {
            delete worker;
            return;
        }
    }

    void tick(ViewController *views) override {
        auto bus = get_board()->i2c_core();
        auto display = get_display();
        display->self_check(self_check_callbacks_.screen());
    }

};

}
