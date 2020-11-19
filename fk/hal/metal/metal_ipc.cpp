#include <os.h>

#include "hal/hal.h"
#include "hal/metal/metal.h"

#include "tasks/tasks.h"

#if defined(FK_HARDWARE_FULL)

namespace fk {

FK_DECLARE_LOGGER("ipc");

MetalMutex storage_mutex;
MetalMutex modules_mutex;
MetalMutex sd_mutex;
MetalRwLock data_lock;

os_queue_define(activity_queue, 10, OS_QUEUE_FLAGS_QUEUE_ONLY);
os_queue_define(button_queue, 10, OS_QUEUE_FLAGS_NONE);
os_queue_define(topology_queue, 10, OS_QUEUE_FLAGS_QUEUE_ONLY);

MetalIPC::MetalIPC() {
    for (auto i = 0u; i < NumberOfWorkerTasks; ++i) {
        running_[i] = WorkerCategory::None;
        workers_[i] = nullptr;
    }
}

bool MetalIPC::available() {
    return os_is_running();
}

bool MetalIPC::begin() {
    OS_CHECK(os_queue_create(os_queue(activity_queue), os_queue_def(activity_queue)));
    OS_CHECK(os_queue_create(os_queue(button_queue), os_queue_def(button_queue)));
    OS_CHECK(os_queue_create(os_queue(topology_queue), os_queue_def(topology_queue)));

    FK_ASSERT(storage_mutex.create());
    FK_ASSERT(modules_mutex.create());
    FK_ASSERT(sd_mutex.create());
    FK_ASSERT(data_lock.create());

    return true;
}

bool MetalIPC::enqueue_activity(Activity *ptr) {
    auto tuple = os_queue_enqueue(os_queue(activity_queue), ptr, 0);
    return tuple.status == OSS_SUCCESS;
}

bool MetalIPC::dequeue_activity(Activity **ptr) {
    auto tuple = os_queue_dequeue(os_queue(activity_queue), 250);
    if (tuple.status != OSS_SUCCESS) {
        return false;
    }
    *ptr = reinterpret_cast<Activity*>(tuple.value.ptr);
    return true;
}

bool MetalIPC::enqueue_button(Button *ptr) {
    auto tuple = os_queue_enqueue(os_queue(button_queue), ptr, 0);
    return tuple.status == OSS_SUCCESS;
}

bool MetalIPC::dequeue_button(Button **ptr) {
    auto tuple = os_queue_dequeue(os_queue(button_queue), OneSecondMs);
    if (tuple.status != OSS_SUCCESS) {
        return false;
    }
    *ptr = reinterpret_cast<Button*>(tuple.value.ptr);
    return true;
}

bool MetalIPC::enqueue_topology(Activity *ptr) {
    auto tuple = os_queue_enqueue(os_queue(topology_queue), ptr, 0);
    return tuple.status == OSS_SUCCESS;
}

bool MetalIPC::dequeue_topology(Activity **ptr, uint32_t to) {
    auto tuple = os_queue_dequeue(os_queue(topology_queue), to);
    if (tuple.status != OSS_SUCCESS) {
        return false;
    }
    *ptr = reinterpret_cast<Activity*>(tuple.value.ptr);
    return true;
}

bool MetalIPC::can_launch(WorkerCategory category) {
    if (category == WorkerCategory::None) {
        return true;
    }

    for (auto i = 0u; i < NumberOfWorkerTasks; ++i) {
        if (os_task_is_running(&worker_tasks[i])) {
            if (running_[i] == category) {
                return false;
            }
        }
    }

    return true;
}

bool MetalIPC::launch_worker(WorkerCategory category, TaskWorker *worker) {
    if (!can_launch(category)) {
        logwarn("unable to launch, already running");
        logwarn("deleting 0x%p", worker);
        delete worker;
        return false;
    }

    for (auto i = 0u; i < NumberOfWorkerTasks; ++i) {
        if (!os_task_is_running(&worker_tasks[i])) {
            worker_tasks[i].name = worker->name();
            running_[i] = category;
            workers_[i] = worker;

            auto priority = worker->priority();
            OS_CHECK(os_task_start_options(&worker_tasks[i], priority, worker));

            return true;
        }
    }

    logwarn("all workers are busy");
    logwarn("deleting 0x%p", worker);

    delete worker;

    return false;
}

bool MetalIPC::remove_worker(TaskWorker *worker) {
    for (auto i = 0u; i < NumberOfWorkerTasks; ++i) {
        if (workers_[i] == worker) {
            workers_[i] = nullptr;
            return true;
        }
    }

    return false;
}

bool MetalIPC::signal_workers(WorkerCategory category, uint32_t signal) {
    logdebug("signaling workers (%" PRIu32 ")", signal);

    FK_DISABLE_IRQ();

    for (auto i = 0u; i < NumberOfWorkerTasks; ++i) {
        if (os_task_is_running(&worker_tasks[i])) {
            if (running_[i] == category) {
                loginfo("signaling worker %zd (%" PRIu32 ")", i, signal);
                os_signal(&worker_tasks[i], 9);
            }
        }
    }

    FK_ENABLE_IRQ();

    return true;
}

collection<TaskDisplayInfo> MetalIPC::get_workers_display_info(Pool &pool) {
    collection<TaskDisplayInfo> infos{ pool };

    FK_DISABLE_IRQ();

    for (auto i = 0u; i < NumberOfWorkerTasks; ++i) {
        if (os_task_is_running(&worker_tasks[i])) {
            if (workers_[i] != nullptr) {
                FK_ASSERT_ADDRESS(workers_[i]);
                infos.emplace(workers_[i]->display_info());
            }
        }
    }

    FK_ENABLE_IRQ();

    return infos;
}

bool MetalIPC::has_running_worker(WorkerCategory category) {
    auto found = false;

    FK_DISABLE_IRQ();

    for (auto i = 0u; i < NumberOfWorkerTasks; ++i) {
        if (os_task_is_running(&worker_tasks[i])) {
            if (running_[i] == category) {
                found = true;
                break;
            }
        }
    }

    FK_ENABLE_IRQ();

    return found;
}

bool MetalIPC::has_any_running_worker() {
    auto found = false;

    FK_DISABLE_IRQ();

    for (auto i = 0u; i < NumberOfWorkerTasks; ++i) {
        if (os_task_is_running(&worker_tasks[i])) {
            found = true;
            break;
        }
    }

    FK_ENABLE_IRQ();

    return found;
}

void MetalIPC::verify() {
    for (auto i = 0u; i < NumberOfWorkerTasks; ++i) {
        if (workers_[i] != nullptr) {
            FK_ASSERT_ADDRESS(workers_[i]);
        }
    }
}

bool MetalMutex::create() {
    return os_mutex_create(&mutex_, &def_) == OSS_SUCCESS;
}

Lock MetalMutex::acquire(uint32_t to) {
    if (!os_is_running()) {
        return Lock{ this };
    }
    if (os_mutex_acquire(&mutex_, to) == OSS_SUCCESS) {
        return Lock{ this };
    }
    return Lock{ nullptr };
}

bool MetalMutex::release() {
    if (!os_is_running()) {
        return true;
    }
    os_mutex_release(&mutex_);
    return true;
}

bool MetalMutex::is_owner() {
    return os_mutex_is_owner(&mutex_) == OSS_SUCCESS;
}

bool MetalRwLock::create() {
    return os_rwlock_create(&rwlock_, &def_) == OSS_SUCCESS;
}

Lock MetalRwLock::acquire_read(uint32_t to) {
    if (!os_is_running()) {
        return Lock{ this };
    }
    if (os_rwlock_acquire_read(&rwlock_, to) == OSS_SUCCESS) {
        logtrace("read!");
        return Lock{ this };
    }
    return Lock{ nullptr };
}

Lock MetalRwLock::acquire_write(uint32_t to) {
    if (!os_is_running()) {
        return Lock{ this };
    }
    if (os_rwlock_acquire_write(&rwlock_, to) == OSS_SUCCESS) {
        logtrace("write!");
        return Lock{ this };
    }
    return Lock{ nullptr };
}

bool MetalRwLock::release() {
    if (!os_is_running()) {
        return true;
    }
    logtrace("release!");
    return os_rwlock_release(&rwlock_) == OSS_SUCCESS;
}

}

#endif
