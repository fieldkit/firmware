#pragma once

#include "common.h"
#include "worker.h"

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

public:
    virtual bool launch_worker(Worker *worker) = 0;

};

IPC *get_ipc();

}
