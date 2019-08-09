#pragma once

#include "common.h"
#include "worker.h"
#include "state.h"

namespace fk {

class Button;
class Activity;

class IPC {
public:
    virtual bool available() = 0;
    virtual bool begin() = 0;

    virtual bool enqueue_activity(Activity *ptr) = 0;
    virtual bool dequeue_activity(Activity **ptr) = 0;

    virtual bool enqueue_button(Button *ptr) = 0;
    virtual bool dequeue_button(Button **ptr) = 0;

    virtual bool enqueue_data(StateChange *ptr) = 0;
    virtual bool dequeue_data(StateChange **ptr) = 0;

    template<typename T>
    bool enqueue_data(T fn) {
        StateChange *sc = new SimpleStateChange<T>(os_task_name(), fn);
        return enqueue_data(sc);
    }

public:
    virtual bool launch_worker(Worker *worker) = 0;

};

IPC *get_ipc();

}
