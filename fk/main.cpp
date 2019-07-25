#include <loading.h>
#include <os.h>

#include "platform.h"
#include "hal/metal/metal.h"

#include "self_check.h"
#include "factory_wipe.h"
#include "scanning.h"

#include "tasks/tasks.h"

extern const struct fkb_header_t fkb_header;

using namespace fk;

FK_DECLARE_LOGGER("main");

static void run_tasks() {
    /**
     * This is very deliberate. By placing these on the stack this way, we
     * ensure that the stack pointer relative to the heap location is as
     * expected by code in the standard library.
     *
     * Declaring these static, for example, will cause them to be placed in the
     * .data section, which is below the heap in memory.
     */
    uint32_t idle_stack[768 / sizeof(uint32_t)];
    uint32_t scheduler_stack[1024 / sizeof(uint32_t)];
    uint32_t display_stack[2048 / sizeof(uint32_t)];
    uint32_t network_stack[(4096 + 1024) / sizeof(uint32_t)];
    uint32_t gps_stack[2048 / sizeof(uint32_t)];
    uint32_t readings_stack[4096 / sizeof(uint32_t)];
    uint32_t data_stack[768 / sizeof(uint32_t)];
    uint32_t worker_stack[2048 / sizeof(uint32_t)];

    OS_CHECK(os_initialize());

    OS_CHECK(os_task_initialize(&idle_task, "idle", OS_TASK_START_RUNNING, &task_handler_idle, NULL, idle_stack, sizeof(idle_stack)));
    OS_CHECK(os_task_initialize(&scheduler_task, "scheduler", OS_TASK_START_RUNNING, &task_handler_scheduler, NULL, scheduler_stack, sizeof(scheduler_stack)));
    if (false) {
        OS_CHECK(os_task_initialize(&display_task, "display", OS_TASK_START_RUNNING, &task_handler_display, NULL, display_stack, sizeof(display_stack)));
    }
    OS_CHECK(os_task_initialize(&network_task, "network", OS_TASK_START_RUNNING, &task_handler_network, NULL, network_stack, sizeof(network_stack)));
    OS_CHECK(os_task_initialize(&gps_task, "gps", OS_TASK_START_RUNNING, &task_handler_gps, NULL, gps_stack, sizeof(gps_stack)));

    os_task_options_t readings_task_options = {
        "readings",
        OS_TASK_START_SUSPENDED,
        task_handler_readings,
        NULL,
        readings_stack,
        sizeof(readings_stack),
        OS_PRIORITY_NORMAL + 2
    };
    OS_CHECK(os_task_initialize_options(&readings_task, &readings_task_options));

    os_task_options_t worker_task_options = {
        "worker",
        OS_TASK_START_SUSPENDED,
        task_handler_worker,
        NULL,
        worker_stack,
        sizeof(worker_stack),
        OS_PRIORITY_NORMAL + 4
    };
    OS_CHECK(os_task_initialize_options(&worker_task, &worker_task_options));

    os_task_options_t data_task_options = {
        "data",
        OS_TASK_START_RUNNING,
        task_handler_data,
        NULL,
        data_stack,
        sizeof(data_stack),
        OS_PRIORITY_NORMAL - 4
    };
    OS_CHECK(os_task_initialize_options(&data_task, &data_task_options));

    auto total_stacks = sizeof(idle_stack) + sizeof(display_stack) + sizeof(network_stack) + sizeof(gps_stack) + sizeof(readings_stack) + sizeof(scheduler_stack) + sizeof(worker_stack);
    loginfo("stacks = %d", total_stacks);
    loginfo("free = %lu", fk_free_memory());
    loginfo("starting os!");

    FK_ASSERT(get_ipc()->begin());

    OS_CHECK(os_start());
}

static size_t write_log(const LogMessage *m, const char *fstring, va_list args) {
    const char *f;
    if ((LogLevels)m->level == LogLevels::ERROR) {
        f = RTT_CTRL_TEXT_GREEN "%08" PRIu32 RTT_CTRL_TEXT_RED " %-6s %s: ";
    }
    else {
        f = RTT_CTRL_TEXT_GREEN "%08" PRIu32 RTT_CTRL_TEXT_YELLOW " %-6s %s" RTT_CTRL_RESET ": ";
    }

    SEGGER_RTT_LOCK();

    auto level = alog_get_log_level((LogLevels)m->level);
    SEGGER_RTT_printf(0, f, m->uptime, level, m->facility);
    SEGGER_RTT_vprintf(0, fstring, &args);
    SEGGER_RTT_WriteString(0, RTT_CTRL_RESET "\n");

    SEGGER_RTT_UNLOCK();

    return true;
}

static void log_diagnostics() {
    loginfo("hello (memory = %lu)", fk_free_memory());

    fk_serial_number_t sn;
    fk_serial_number_get(&sn);
    loginfo("serial = %08x-%08x-%08x-%08x", sn.dwords[0], sn.dwords[1], sn.dwords[2], sn.dwords[3]);

    loginfo("fw = %s", fkb_header.firmware.name);
    char hash_string[128];
    bytes_to_hex_string(hash_string, sizeof(hash_string), fkb_header.firmware.hash, fkb_header.firmware.hash_size);
    loginfo("hash = %s", hash_string);
}

static void initialize_hardware() {
    get_board()->initialize();
    // NOTE: We do this ASAP because the GPIO on the modmux can be in any state.
    get_modmux()->begin();
    FK_ASSERT(fk_random_initialize() == 0);
    FK_ASSERT(get_buttons()->begin());
}

void setup() {
    log_configure_writer(write_log);
    log_configure_level(LogLevels::DEBUG);
    log_diagnostics();

    initialize_hardware();

    auto display = get_display();
    SelfCheck self_check(display, get_network());

    self_check.check();

    Storage storage{ MemoryFactory::get_data_memory() };
    FactoryWipe fw{ get_buttons(), &storage };
    FK_ASSERT(fw.wipe_if_necessary());

    ModuleScanning scanning{ get_modmux() };
    FK_ASSERT(scanning.scan());

    storage.fsck();

    if (fkc.slow_startup) {
        fk_delay(1000);

        display->fk_logo();

        fk_delay(1000);
    }

    // Call this here because things go horribly if we call from within a task.
    // Something goes south with a malloc.
    random(100, 1000);

    run_tasks();
}

void loop() {
}
