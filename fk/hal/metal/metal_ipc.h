#pragma once

#if defined(FK_HARDWARE_FULL)

#include <os.h>

#include "hal/hal.h"

namespace fk {

class MetalIPC : public IPC {
public:
    MetalIPC();

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
        ~Lock() {
            if (mutex_ != nullptr) {
                mutex_->release();
                mutex_ = nullptr;
            }
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

extern Mutex storage_mutex;
extern Mutex peripheral_i2c_core_mutex;
extern Mutex spi_flash_mutex;

}

#endif
