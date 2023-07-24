#include "hal.h"
#include "hal/metal/metal_ipc.h"
#include "hal/linux/linux_ipc.h"

namespace fk {

class SingleThreadedIPC : public IPC {
public:
    bool available() override {
        return true;
    }

    bool begin() override {
        return true;
    }

    bool enqueue_activity(Activity *ptr) override {
        return true;
    }
    bool dequeue_activity(Activity **ptr) override {
        return true;
    }

    bool enqueue_button(Button *ptr) override {
        return true;
    }
    bool dequeue_button(Button **ptr) override {
        return true;
    }

    bool enqueue_topology(Activity *ptr) override {
        return true;
    }
    bool dequeue_topology(Activity **ptr, uint32_t to) override {
        return true;
    }

public:
    bool launch_worker(WorkerCategory category, TaskWorker *worker, bool concurrency_allowed) override {
        worker->run();

        return true;
    }

    bool fork_worker(WorkerCategory category, TaskWorker *worker) override {
        return true;
    }
};

#if defined(FK_HARDWARE_FULL)
#if defined(FK_IPC_SINGLE_THREADED)
static SingleThreadedIPC ipc;
#else
static MetalIPC ipc;
#endif
#else
static LinuxIPC ipc;
#endif

IPC *get_ipc() {
    return &ipc;
}

} // namespace fk
