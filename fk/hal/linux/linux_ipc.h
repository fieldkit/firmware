#pragma once

#if defined(linux)

#include "hal/ipc.h"

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
    bool enqueue_topology(Activity *ptr) override;
    bool dequeue_topology(Activity **ptr, uint32_t to) override;

public:
    bool launch_worker(WorkerCategory category, TaskWorker *worker) override;

};

class LinuxMutex : public Mutex {
public:
    bool create() override;
    Lock acquire(uint32_t to) override;
    bool release() override;
    bool is_owner() override;

};

class LinuxRwLock : public RwLock {
public:
    bool create() override;
    Lock acquire_read(uint32_t to) override;
    Lock acquire_write(uint32_t to) override;
    bool release() override;

};

extern LinuxMutex storage_mutex;
extern LinuxMutex modules_mutex;
extern LinuxMutex wifi_mutex;
extern LinuxMutex sd_mutex;
extern LinuxRwLock data_lock;

}

#endif
