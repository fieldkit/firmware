#pragma once

#if defined(linux)

#include "hal/hal.h"

namespace fk {

class LinuxIPC : public IPC {
public:
    LinuxIPC();

public:
    bool available() override;
    bool begin() override;
    bool enqueue_activity(Activity *ptr) override;
    bool dequeue_activity(Activity **ptr) override;
    bool enqueue_button(Button *ptr) override;
    bool dequeue_button(Button **ptr) override;
    bool enqueue_data(StateChange *ptr) override;
    bool dequeue_data(StateChange **ptr) override;

public:
    bool launch_worker(Worker *worker) override;

};

}

#endif
