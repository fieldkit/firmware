#include "hal/hal.h"
#include "hal/linux/linux.h"

#if defined(linux)

namespace fk {

LinuxIPC::LinuxIPC() {
}

bool LinuxIPC::available() {
    return false;
}

bool LinuxIPC::begin() {
    return false;
}

bool LinuxIPC::enqueue(void *ptr, uint32_t to) {
    return false;
}

bool LinuxIPC::dequeue(void **ptr, uint32_t to) {
    return false;
}

bool LinuxIPC::enqueue_data(void *ptr, uint32_t to) {
    return false;
}

bool LinuxIPC::dequeue_data(void **ptr, uint32_t to) {
    return false;
}

bool LinuxIPC::launch_worker(Worker *worker) {
    FK_ASSERT(worker != nullptr);

    WorkerContext wc;
    worker->run(wc);
    delete worker;

    return true;
}

}

#endif
