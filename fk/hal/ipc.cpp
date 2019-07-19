#include <os.h>

#include "hal.h"
#include "tasks/tasks.h"

namespace fk {

FK_DECLARE_LOGGER("ipc");

static IPC ipc;

IPC *get_ipc() {
    return &ipc;
}

#if defined(FK_HARDWARE_FULL)

os_queue_define(other_queue, 10, OS_QUEUE_FLAGS_NONE);
os_queue_define(data_queue, 10, OS_QUEUE_FLAGS_QUEUE_ONLY);

bool IPC::available() {
    return os_is_running();
}

bool IPC::begin() {
    OS_CHECK(os_queue_create(os_queue(other_queue), os_queue_def(other_queue)));
    OS_CHECK(os_queue_create(os_queue(data_queue), os_queue_def(data_queue)));

    return true;
}

bool IPC::enqueue(void *ptr, uint32_t to) {
    auto tuple = os_queue_enqueue(os_queue(other_queue), ptr, to);
    return tuple.status == OSS_SUCCESS;
}

bool IPC::dequeue(void **ptr, uint32_t to) {
    auto tuple = os_queue_dequeue(os_queue(other_queue), to);
    if (tuple.status != OSS_SUCCESS) {
        return false;
    }
    *ptr = tuple.value.ptr;
    return true;
}

bool IPC::enqueue_data(void *ptr, uint32_t to) {
    auto tuple = os_queue_enqueue(os_queue(data_queue), ptr, to);
    return tuple.status == OSS_SUCCESS;
}

bool IPC::dequeue_data(void **ptr, uint32_t to) {
    auto tuple = os_queue_dequeue(os_queue(data_queue), to);
    if (tuple.status != OSS_SUCCESS) {
        return false;
    }
    *ptr = tuple.value.ptr;
    return true;
}

bool IPC::launch_worker(Worker *worker) {
    if (os_task_is_running(&worker_task)) {
        logwarn("all workers are busy");
        return false;
    }

    OS_CHECK(os_task_start_options(&worker_task, worker));

    return true;
}

#else

bool IPC::available() {
    return false;
}

bool IPC::begin() {
    return false;
}

bool IPC::enqueue(void *ptr, uint32_t to) {
    return false;
}

bool IPC::dequeue(void **ptr, uint32_t to) {
    return false;
}

bool IPC::enqueue_data(void *ptr, uint32_t to) {
    return false;
}

bool IPC::dequeue_data(void **ptr, uint32_t to) {
    return false;
}

bool IPC::launch_worker(Worker *worker) {
    FK_ASSERT(worker != nullptr);

    WorkerContext wc;
    worker->run(wc);

    return true;
}

#endif

}

