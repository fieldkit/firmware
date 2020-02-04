#include "tasks/tasks.h"
#include "hal/hal.h"
#include "clock.h"
#include "protobuf.h"
#include "storage/storage.h"

namespace fk {

FK_DECLARE_LOGGER("task");

void task_handler_worker(void *params) {
    FK_ASSERT(params != nullptr);

    auto worker = reinterpret_cast<TaskWorker*>(params);
    auto started = fk_uptime();
    worker->run();
    delete worker;

    // Highwater is kind of a nisnomer because it's the number of
    // dwords still remaining on the stack. So we're gonna tell the
    // user how much was left.
    auto self = os_task_self();
    auto bytes_left = os_task_highwater(self) * sizeof(uint32_t);
    auto stack_percentage = bytes_left / (float)self->stack_size * 100.0f;

    loginfo("done (%" PRIu32 "ms) stack: %" PRIu32 " / %" PRIu32 " bytes %.2f%% free",
            fk_uptime() - started, self->stack_size, bytes_left, stack_percentage);
}

}
