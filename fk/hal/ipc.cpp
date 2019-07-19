#include <os.h>

#include "hal.h"

namespace fk {

static IPC ipc;

IPC *get_ipc() {
    return &ipc;
}

#if defined(FK_HARDWARE_FULL)

os_queue_define(queue, 10);
os_queue_define(data_queue, 10);

bool IPC::begin() {
    OS_CHECK(os_queue_create(os_queue(queue), os_queue_def(queue)));
    OS_CHECK(os_queue_create(os_queue(data_queue), os_queue_def(data_queue)));

    return true;
}

bool IPC::enqueue(void *ptr, uint32_t to) {
    auto tuple = os_queue_enqueue(os_queue(queue), ptr, to);
    return tuple.status == OSS_SUCCESS;
}

bool IPC::dequeue(void **ptr, uint32_t to) {
    auto tuple = os_queue_dequeue(os_queue(queue), to);
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
bool IPC::available() {
    return os_is_running();
}

#else

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

bool IPC::available() {
    return false;
}

#endif

}

