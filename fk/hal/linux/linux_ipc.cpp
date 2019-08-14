#include "hal/hal.h"
#include "hal/linux/linux.h"

#if defined(linux)

namespace fk {

FK_DECLARE_LOGGER("ipc");

Mutex storage_mutex;
Mutex peripheral_i2c_core_mutex;

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

bool LinuxIPC::enqueue_data(StateChange *ptr) {
    return false;
}

bool LinuxIPC::dequeue_data(StateChange **ptr) {
    return false;
}

bool LinuxIPC::launch_worker(Worker *worker) {
    FK_ASSERT(worker != nullptr);

    WorkerContext wc;
    worker->run(wc);
    delete worker;

    return true;
}

bool Mutex::create() {
    return true;
}

Mutex::Lock Mutex::acquire(uint32_t to) {
    loginfo("acquired");
    return { this };
}

bool Mutex::release() {
    loginfo("release");
    return false;
}

}

#endif
