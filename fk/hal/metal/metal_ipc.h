#pragma once

#if defined(FK_HARDWARE_FULL)

#include <os.h>

#include "hal/ipc.h"
#include "hal/mutex.h"
#include "config.h"

namespace fk {

class MetalIPC : public IPC {
private:
    WorkerCategory running_[NumberOfWorkerTasks];
    TaskWorker *workers_[NumberOfWorkerTasks];

public:
    MetalIPC();

public:
    bool available() override;
    bool begin() override;
    bool enqueue_activity(Activity *ptr) override;
    bool dequeue_activity(Activity **ptr) override;
    bool enqueue_button(Button *ptr) override;
    bool dequeue_button(Button **ptr) override;
    bool enqueue_topology(Activity *ptr) override;
    bool dequeue_topology(Activity **ptr, uint32_t to = FiveSecondsMs) override;

public:
    bool launch_worker(WorkerCategory category, TaskWorker *worker) override;
    bool signal_workers(WorkerCategory category, uint32_t signal) override;
    collection<TaskDisplayInfo> get_workers_display_info(Pool &pool) override;

private:
    bool can_launch(WorkerCategory category);

};

class NoopMutex {
public:
    class Lock {
    public:
        Lock() {
        }
        virtual ~Lock() {
        }

    public:
        operator bool() {
            return true;
        }
    };

public:
    bool create() {
        return true;
    }

    Lock acquire(uint32_t to) {
        return Lock{ };
    }

    bool release() {
        return true;
    }

    bool is_owner() {
        return true;
    }
};

class MetalMutex : public Mutex {
private:
    os_mutex_definition_t def_;
    os_mutex_t mutex_;

public:
    bool create() override;
    Lock acquire(uint32_t to) override;
    bool release() override;
    bool is_owner() override;

};

class MetalRwLock : public RwLock {
private:
    os_rwlock_definition_t def_;
    os_rwlock_t rwlock_;

public:
    bool create() override;
    Lock acquire_read(uint32_t to) override;
    Lock acquire_write(uint32_t to) override;
    bool release() override;

};

extern MetalMutex storage_mutex;
extern MetalMutex modules_mutex;
extern MetalMutex sd_mutex;
extern MetalRwLock data_lock;

}

#endif
