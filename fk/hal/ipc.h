#pragma once

#include "common.h"
#include "worker.h"
#include "collections.h"

namespace fk {

class Button;
class Activity;

enum class WorkerCategory  {
    None,
    Readings,
    Storage,
    Transfer,
    Lora,
};

class IPC {
public:
    virtual bool available() = 0;
    virtual bool begin() = 0;

    virtual bool enqueue_activity(Activity *ptr) = 0;
    virtual bool dequeue_activity(Activity **ptr) = 0;

    virtual bool enqueue_button(Button *ptr) = 0;
    virtual bool dequeue_button(Button **ptr) = 0;

    virtual bool enqueue_topology(Activity *ptr) = 0;
    virtual bool dequeue_topology(Activity **ptr, uint32_t to) = 0;

    virtual void verify() {
    }

public:
    virtual bool launch_worker(WorkerCategory category, TaskWorker *worker) = 0;

    virtual bool launch_worker(TaskWorker *worker) {
        return launch_worker(WorkerCategory::None, worker);
    }

    virtual bool remove_worker(TaskWorker *worker) {
        return false;
    }

    virtual bool signal_workers(WorkerCategory category, uint32_t signal) {
        return true;
    }

    virtual bool has_any_running_worker() {
        return false;
    }

    virtual bool has_running_worker(WorkerCategory category) {
        return false;
    }

    virtual collection<TaskDisplayInfo> get_workers_display_info(Pool &pool) {
        return { pool };
    }

};

IPC *get_ipc();

} // namespace fk
