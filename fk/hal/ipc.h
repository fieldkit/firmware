#pragma once

#include "common.h"
#include "worker.h"

namespace fk {

class Button;
class Activity;

enum class WorkerCategory  {
    None,
    Readings,
    Storage,
    Transfer,
};

class IPC {
public:
    virtual bool available() = 0;
    virtual bool begin() = 0;

    virtual bool enqueue_activity(Activity *ptr) = 0;
    virtual bool dequeue_activity(Activity **ptr) = 0;

    virtual bool enqueue_button(Button *ptr) = 0;
    virtual bool dequeue_button(Button **ptr) = 0;

public:
    virtual bool launch_worker(WorkerCategory category, TaskWorker *worker) = 0;

    virtual bool launch_worker(TaskWorker *worker) {
        return launch_worker(WorkerCategory::None, worker);
    }

};

IPC *get_ipc();

}
