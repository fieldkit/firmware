#include <malloc.h>
#include <loading.h>
#include <os.h>

#include "platform.h"
#include "utilities.h"
#include "hal/metal/metal.h"
#include "hal/flash.h"
#include "debugging.h"
#include "storage/storage.h"
#include "tasks/tasks.h"
#include "state.h"
#include "logging.h"
#include "status_logging.h"
#include "live_tests.h"
#include "startup_worker.h"
#include "modules/dyn/process.h"

extern const struct fkb_header_t fkb_header;

using namespace fk;

FK_DECLARE_LOGGER("main");

static void run_tasks() {
    uint32_t stack_size = (4096 + 2048) / sizeof(uint32_t);

    /**
     * This is very deliberate. By placing these on the stack this way, we
     * ensure that the stack pointer relative to the heap location is as
     * expected by code in the standard library.
     *
     * Declaring these static, for example, will cause them to be placed in the
     * .data section, which is below the heap in memory.
     */
    uint32_t idle_stack[stack_size];
    uint32_t scheduler_stack[stack_size];
    uint32_t display_stack[stack_size];
    uint32_t gps_stack[stack_size];
    // uint32_t debug_stack[128];
    uint32_t worker_stacks[NumberOfWorkerTasks][stack_size];
    uint32_t network_stack[stack_size];

    auto total_stacks = sizeof(idle_stack) +
        sizeof(scheduler_stack) +
        sizeof(display_stack) +
        sizeof(gps_stack) +
        // sizeof(debug_stack) +
        sizeof(worker_stacks) +
        sizeof(network_stack);

    os_task_options_t display_task_options = {
        "display",
        OS_TASK_START_SUSPENDED,
        task_handler_display,
        nullptr,
        display_stack,
        sizeof(display_stack),
        OS_PRIORITY_NORMAL + 2
    };

    OS_CHECK(os_initialize());

    OS_CHECK(os_task_initialize(&idle_task, "idle", OS_TASK_START_RUNNING, &task_handler_idle, nullptr, idle_stack, sizeof(idle_stack)));
    OS_CHECK(os_task_initialize(&scheduler_task, "scheduler", OS_TASK_START_SUSPENDED, &task_handler_scheduler, nullptr, scheduler_stack, sizeof(scheduler_stack)));
    OS_CHECK(os_task_initialize(&network_task, "network", OS_TASK_START_SUSPENDED, &task_handler_network, nullptr, network_stack, sizeof(network_stack)));
    #if defined(FK_ENABLE_DEBUG_TASK)
    // NOTICE NOTICE We share GPS stack! NOTICE NOTICE
    OS_CHECK(os_task_initialize(&debug_task, "debug", OS_TASK_START_SUSPENDED, &task_handler_debug, nullptr, gps_stack, sizeof(gps_stack)));
    #else
    OS_CHECK(os_task_initialize(&gps_task, "gps", OS_TASK_START_SUSPENDED, &task_handler_gps, nullptr, gps_stack, sizeof(gps_stack)));
    #endif

    OS_CHECK(os_task_initialize_options(&display_task, &display_task_options));

    for (size_t i = 0; i < NumberOfWorkerTasks; ++i) {
        OS_CHECK(os_task_initialize(&worker_tasks[i], "worker", OS_TASK_START_SUSPENDED, &task_handler_worker, nullptr, worker_stacks[i], sizeof(worker_stacks[i])));
    }

    FK_ASSERT(get_ipc()->begin());

    FK_ASSERT(get_ipc()->launch_worker(create_pool_worker<StartupWorker>()));

    auto mi = mallinfo();
    loginfo("memory arena = %zd used = %zd", (size_t)mi.arena, (size_t)mi.uordblks);
    loginfo("stacks = %d", total_stacks);
    loginfo("free = %" PRIu32, fk_free_memory());
    loginfo("starting os!");

    OS_CHECK(os_start());
}

static bool initialize_backplane() {
    if (get_module_leds()->begin()) {
        get_module_leds()->off();
    }

    if (!get_modmux()->begin()) {
        logerror("no backplane!");

        get_board()->i2c_module().recover();

        if (get_modmux()->begin()) {
            loginfo("i2c recover worked!");
        }
    }

    return true;
}

static bool initialize_hardware() {
    FK_ASSERT(get_board()->initialize());

    FK_ASSERT(get_buttons()->begin());

    FK_ASSERT(fk_random_initialize() == 0);

    FK_ASSERT(get_flash()->initialize());

    get_board()->enable_everything();

    fk_delay(10);

    initialize_backplane();

    return true;
}

static void single_threaded_setup() {
    FK_ASSERT(fk_logging_initialize());

    FK_ASSERT(initialize_hardware());

    FK_ASSERT(fk_debugging_initialize());

    FK_ASSERT(fk_log_diagnostics());
}

void setup() {
    single_threaded_setup();
    fk_live_tests();
    run_tasks();
}

void loop() {
}
