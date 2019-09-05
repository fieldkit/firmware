#include "hal/hal.h"
#include "hal/linux/linux.h"

#if defined(linux)

namespace fk {

FK_DECLARE_LOGGER("ipc");

Mutex storage_mutex;
Mutex peripheral_i2c_core_mutex;
RwLock data_lock;

LinuxIPC::LinuxIPC() {
}

bool LinuxIPC::available() {
    return false;
}

bool LinuxIPC::begin() {
    return false;
}

bool LinuxIPC::enqueue_activity(Activity *ptr) {
    return false;
}

bool LinuxIPC::dequeue_activity(Activity **ptr) {
    return false;
}

bool LinuxIPC::enqueue_button(Button *ptr) {
    return false;
}

bool LinuxIPC::dequeue_button(Button **ptr) {
    return false;
}

bool LinuxIPC::launch_worker(WorkerCategory category, Worker *worker) {
    FK_ASSERT(worker != nullptr);

    worker->run();
    delete worker;

    return true;
}

bool Mutex::create() {
    return true;
}

Mutex::Lock Mutex::acquire(uint32_t to) {
    return { this };
}

bool Mutex::release() {
    return true;
}

bool Mutex::is_owner() {
    return false;
}

bool RwLock::create() {
    return true;
}

RwLock::Lock RwLock::acquire_read(uint32_t to) {
    return { this };
}

RwLock::Lock RwLock::acquire_write(uint32_t to) {
    return { this };
}

bool RwLock::release() {
    return true;
}

}

#endif
