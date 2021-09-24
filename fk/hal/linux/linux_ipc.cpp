#include "hal/hal.h"
#include "hal/linux/linux.h"

#if defined(linux)

namespace fk {

FK_DECLARE_LOGGER("ipc");

LinuxMutex storage_mutex;
LinuxMutex modules_mutex;
LinuxMutex sd_mutex;
LinuxMutex wifi_mutex;
LinuxMutex i2c_module_mutex;
LinuxMutex i2c_core_mutex;
LinuxMutex i2c_radio_mutex;
LinuxMutex lora_mutex;
LinuxRwLock data_lock;

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

bool LinuxIPC::enqueue_topology(Activity *ptr) {
    return false;
}

bool LinuxIPC::dequeue_topology(Activity **ptr, uint32_t to) {
    return false;
}

bool LinuxIPC::launch_worker(WorkerCategory category, TaskWorker *worker, bool concurrency_allowed) {
    FK_ASSERT(worker != nullptr);

    worker->run();

    delete worker;

    return true;
}

bool LinuxMutex::create() {
    return true;
}

Lock LinuxMutex::acquire(uint32_t to) {
    return Lock{ this };
}

bool LinuxMutex::release() {
    return true;
}

bool LinuxMutex::is_owner() {
    return false;
}

bool LinuxRwLock::create() {
    return true;
}

Lock LinuxRwLock::acquire_read(uint32_t to) {
    return Lock{ this };
}

Lock LinuxRwLock::acquire_write(uint32_t to) {
    return Lock{ this };
}

bool LinuxRwLock::release() {
    return true;
}

} // namespace fk

#endif
