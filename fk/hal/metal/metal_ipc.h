#pragma once

#if defined(FK_HARDWARE_FULL)

#include <os.h>

#include "hal/ipc.h"
#include "config.h"

namespace fk {

class MetalIPC : public IPC {
private:
    WorkerCategory running_[NumberOfWorkerTasks];

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
    bool dequeue_topology(Activity **ptr) override;

public:
    bool launch_worker(WorkerCategory category, TaskWorker *worker) override;
    bool signal_workers(WorkerCategory category, uint32_t signal);

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

class Mutex {
private:
    os_mutex_definition_t def_;
    os_mutex_t mutex_;

public:
    class Lock {
    private:
        Mutex *mutex_;

    public:
        Lock(Mutex *mutex) : mutex_(mutex) { }
        Lock(Lock &&rhs) : mutex_(std::move(rhs.mutex_)) {
            rhs.mutex_ = nullptr;
        }
        ~Lock() {
            if (mutex_ != nullptr) {
                mutex_->release();
                mutex_ = nullptr;
            }
        }

    public:
        Lock& operator =(Lock&& rhs) {
            if (this != &rhs) {
                mutex_ = rhs.mutex_;
                rhs.mutex_ = nullptr;
            }
            return *this;
        }

    public:
        operator bool() {
            return mutex_ != nullptr;
        }
    };

public:
    bool create();
    Lock acquire(uint32_t to);
    bool release();
    bool is_owner();

};

class RwLock {
private:
    os_rwlock_definition_t def_;
    os_rwlock_t rwlock_;

public:
    class Lock {
    private:
        RwLock *rwlock_;

    public:
        Lock(RwLock *rwlock) : rwlock_(rwlock) { }
        Lock(Lock &&rhs) : rwlock_(std::move(rhs.rwlock_)) {
            rhs.rwlock_ = nullptr;
        }
        ~Lock() {
            if (rwlock_ != nullptr) {
                rwlock_->release();
                rwlock_ = nullptr;
            }
        }

    public:
        Lock& operator =(Lock&& rhs) {
            if (this != &rhs) {
                rwlock_ = rhs.rwlock_;
                rhs.rwlock_ = nullptr;
            }
            return *this;
        }

    public:
        operator bool() {
            return rwlock_ != nullptr;
        }
    };

public:
    bool create();
    Lock acquire_read(uint32_t to);
    Lock acquire_write(uint32_t to);
    bool release();

};

extern Mutex storage_mutex;
extern Mutex peripheral_i2c_core_mutex;
extern RwLock data_lock;

}

#endif
