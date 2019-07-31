#include <os.h>

#include "hal/hal.h"
#include "hal/metal/metal.h"

#include "tasks/tasks.h"

#if defined(FK_HARDWARE_FULL)

namespace fk {

FK_DECLARE_LOGGER("ipc");

os_queue_define(other_queue, 10, OS_QUEUE_FLAGS_NONE);
os_queue_define(data_queue, 10, OS_QUEUE_FLAGS_QUEUE_ONLY);

MetalIPC::MetalIPC() {
}

bool MetalIPC::available() {
    return os_is_running();
}

bool MetalIPC::begin() {
    OS_CHECK(os_queue_create(os_queue(other_queue), os_queue_def(other_queue)));
    OS_CHECK(os_queue_create(os_queue(data_queue), os_queue_def(data_queue)));

    return true;
}

bool MetalIPC::enqueue(void *ptr, uint32_t to) {
    auto tuple = os_queue_enqueue(os_queue(other_queue), ptr, to);
    return tuple.status == OSS_SUCCESS;
}

bool MetalIPC::dequeue(void **ptr, uint32_t to) {
    auto tuple = os_queue_dequeue(os_queue(other_queue), to);
    if (tuple.status != OSS_SUCCESS) {
        return false;
    }
    *ptr = tuple.value.ptr;
    return true;
}

bool MetalIPC::enqueue_data(StateChange *ptr, uint32_t to) {
    auto tuple = os_queue_enqueue(os_queue(data_queue), ptr, to);
    return tuple.status == OSS_SUCCESS;
}

bool MetalIPC::dequeue_data(StateChange **ptr, uint32_t to) {
    auto tuple = os_queue_dequeue(os_queue(data_queue), to);
    if (tuple.status != OSS_SUCCESS) {
        return false;
    }
    *ptr = reinterpret_cast<StateChange*>(tuple.value.ptr);
    return true;
}

bool MetalIPC::launch_worker(Worker *worker) {
    if (os_task_is_running(&worker_task)) {
        logwarn("all workers are busy");
        return false;
    }

    OS_CHECK(os_task_start_options(&worker_task, worker));

    return true;
}

bool Mutex::create() {
    os_mutex_create(&mutex_, &def_);
    return true;
}

Mutex::Lock Mutex::acquire(uint32_t to) {
    if (os_mutex_acquire(&mutex_, to) == OSS_SUCCESS) {
        return { this };
    }
    return { nullptr };
}

bool Mutex::release() {
    os_mutex_release(&mutex_);
    return false;
}

}

#endif
