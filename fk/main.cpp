#include <malloc.h>
#include <loading.h>
#include <os.h>

#include "platform.h"
#include "utilities.h"
#include "hal/metal/metal.h"
#include "hal/flash.h"
#include "debugging.h"
#include "startup_worker.h"
#include "storage/storage.h"
#include "tasks/tasks.h"
#include "state.h"
#include "logging.h"
#include "live_tests.h"

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
    uint32_t worker_stacks[NumberOfWorkerTasks][stack_size];
    uint32_t network_stack[stack_size];

    auto total_stacks = sizeof(idle_stack) +
        sizeof(scheduler_stack) +
        sizeof(display_stack) +
        sizeof(gps_stack) +
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

    OS_CHECK(os_task_initialize(&idle_task, "idle", OS_TASK_START_RUNNING, &task_handler_idle, nullptr, idle_stack, sizeof(idle_stack)));
    OS_CHECK(os_task_initialize(&scheduler_task, "scheduler", OS_TASK_START_SUSPENDED, &task_handler_scheduler, nullptr, scheduler_stack, sizeof(scheduler_stack)));
    OS_CHECK(os_task_initialize(&network_task, "network", OS_TASK_START_SUSPENDED, &task_handler_network, nullptr, network_stack, sizeof(network_stack)));
    OS_CHECK(os_task_initialize(&gps_task, "gps", OS_TASK_START_SUSPENDED, &task_handler_gps, nullptr, gps_stack, sizeof(gps_stack)));

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

extern uint32_t __cm_ram_origin__;
extern uint32_t __cm_ram_end__;
extern uint32_t __heap_start__;
extern uint32_t __heap_end__;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;

static bool log_diagnostics() {
    uint8_t stack_dummy = 0;
    auto in_stack = (uint8_t *)&__cm_ram_end__ - &stack_dummy;
    auto available = fk_free_memory();
    auto data = (&__data_end__ - &__data_start__) * sizeof(uint32_t);
    auto bss = (&__bss_end__ - &__bss_start__) * sizeof(uint32_t);
    auto heap = (&__heap_end__ - &__heap_start__) * sizeof(uint32_t);
    auto used = (&__heap_end__ - &__cm_ram_origin__) * sizeof(uint32_t);

    loginfo("hello (memory = %lu) (data + bss + heap = %zd + %zd + %zd = %zd) (used = %zd) (stack ~ %zd)",
            available, data, bss, heap, data + bss + heap, used, in_stack);

    fk_serial_number_t sn;
    loginfo("serial = %08" PRIx32 "-%08" PRIx32 "-%08" PRIx32 "-%08" PRIx32,
            (uint32_t)__builtin_bswap32(sn.dwords[0]), (uint32_t)__builtin_bswap32(sn.dwords[1]),
            (uint32_t)__builtin_bswap32(sn.dwords[2]), (uint32_t)__builtin_bswap32(sn.dwords[3]));

    loginfo("fw = %s (#%" PRIu32 ")", fkb_header.firmware.name, fkb_header.firmware.number);
    char hash_string[128];
    bytes_to_hex_string(hash_string, sizeof(hash_string), fkb_header.firmware.hash, fkb_header.firmware.hash_size);
    loginfo("hash = %s", hash_string);
    loginfo("version = %s", fkb_header.firmware.version);

    loginfo("sizeof(RecordHeader + RecordTail) = %zd + %zd", sizeof(RecordHeader), sizeof(RecordTail));
    loginfo("sizeof(GlobalState) = %zd", sizeof(GlobalState));

    return true;
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
    OS_CHECK(os_initialize());

    FK_ASSERT(fk_logging_initialize());

    FK_ASSERT(initialize_hardware());

    FK_ASSERT(fk_debugging_initialize());

    FK_ASSERT(log_diagnostics());
}

void setup() {
    single_threaded_setup();
    fk_live_tests();
    run_tasks();
}

void loop() {
}
