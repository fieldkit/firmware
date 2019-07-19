#include <loading.h>
#include <os.h>

#include "board.h"
#include "platform.h"
#include "hal/hal.h"
#include "hal/metal/metal.h"
#include "httpd/server.h"
#include "self_check.h"
#include "storage.h"
#include "clock.h"
#include "protobuf.h"

#include "tiny_printf.h"
#include "secrets.h"

extern const struct fkb_header_t fkb_header;

using namespace fk;

FK_DECLARE_LOGGER("main");

os_task_t idle_task;
os_task_t scheduler_task;
os_task_t display_task;
os_task_t network_task;
os_task_t gps_task;
os_task_t readings_task;

static void task_handler_idle(void *params) {
    while (true) {
        fk_delay(FiveSecondsMs);
    }
}

static void task_handler_scheduler(void *params) {
    auto last_readings = fk_uptime();

    while (true) {
        auto reading = get_battery_gauge()->get();
        if (reading.available) {
            loginfo("battery(%dmv %d%% %dC %fs %fs)", reading.cellv, reading.soc, reading.temp, reading.tte, reading.ttf);
        }

        {
            auto clock = get_clock();
            DateTime internal;
            DateTime external;

            FK_ASSERT(clock->internal(internal));
            FK_ASSERT(clock->external(external));

            FormattedTime internal_formatted{ internal.unixtime() };
            FormattedTime external_formatted{ external.unixtime() };
            loginfo("now: %s %s", internal_formatted.cstr(), external_formatted.cstr());
        }

        void *message = nullptr;
        if (get_ipc()->dequeue(&message, FiveSecondsMs)) {
            auto status = os_task_get_status(&network_task);
            if (status == OS_TASK_STATUS_SUSPENDED || status == OS_TASK_STATUS_FINISHED) {
                loginfo("starting task '%s'", network_task.name);
                os_task_start(&network_task);
            }
        }

        if (fk_uptime() - last_readings > ThirtySecondsMs) {
            auto status = os_task_get_status(&readings_task);
            if (status == OS_TASK_STATUS_SUSPENDED || status == OS_TASK_STATUS_FINISHED) {
                loginfo("starting task '%s'", readings_task.name);
                os_task_start(&readings_task);
            }
            else {
                loginfo("task is still busy %s", readings_task.name);
            }
            last_readings = fk_uptime();
        }
    }
}

static void task_handler_display(void *params) {
    DisplayFactory display_factory;
    Display *display = display_factory.get_display();

    while (true) {
        home_screen_t screen = {
            .time = fk_uptime(),
            .wifi = true,
            .gps = true,
            .battery = 1.0f,
        };

        display->home(screen);

        fk_delay(1000);
    }
}

static void task_handler_network(void *params) {
    MetalNetwork network;
    #if defined(FK_WIFI_0_SSID) && defined(FK_WIFI_0_PASSWORD)
    HttpServer http_server{ &network, FK_WIFI_0_SSID, FK_WIFI_0_PASSWORD };
    #else
    HttpServer http_server{ &network };
    #endif

    if (!http_server.begin()) {
        logerror("error starting server");
        return;
    }

    while (http_server.active_connections() || fk_uptime() - http_server.activity() < fkc.network.uptime) {
        http_server.tick();
        fk_delay(10);
    }

    loginfo("network stopped");
}

static void task_handler_gps(void *params) {
    auto gps = get_gps();

    if (!gps->begin()) {
        logerror("gps failed");
        return;
    }

    while (true) {
        GpsFix fix;
        gps->service(fix);
        fk_delay(10);
    }
}

static void task_handler_readings(void *params) {
    auto memory = MemoryFactory::get_data_memory();
    Storage storage{ memory };

    if (!storage.begin()) {
        return;
    }

    auto now = get_clock()->now().unixtime();

    auto file = storage.file(0);

    fk_data_SensorAndValue readings[] = {
        { 0, (float)random(20, 100) },
        { 1, (float)random(20, 100) },
        { 2, (float)random(20, 100) },
        { 3, (float)random(20, 100) },
        { 4, (float)random(20, 100) },
        { 5, (float)random(20, 100) },
        { 6, (float)random(20, 100) },
        { 7, (float)random(20, 100) },
        { 8, (float)random(20, 100) },
        { 9, (float)random(20, 100) },
    };

    pb_array_t readings_array = {
        .length = (size_t)10,
        .itemSize = sizeof(fk_data_SensorAndValue),
        .buffer = &readings,
        .fields = fk_data_SensorAndValue_fields,
    };

    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.readings.time = now;
    record.readings.reading = file.record();
    record.readings.flags = 0;
    record.readings.location.fix = 0;
    record.readings.location.time = now;
    record.readings.location.longitude = -118.2709223;
    record.readings.location.latitude = 34.0318047;
    record.readings.location.altitude = 100.0f;
    record.readings.readings.funcs.encode = pb_encode_array;
    record.readings.readings.arg = &readings_array;

    auto bytes_wrote = file.write(&record, fk_data_DataRecord_fields);
    if (bytes_wrote == 0) {
        logerror("error saving readings");
    }

    loginfo("wrote %d bytes (%d bytes)", bytes_wrote, file.size());
}

void run_tasks() {
    /**
     * This is very deliberate. By placing these on the stack this way, we
     * ensure that the stack pointer relative to the heap location is as
     * expected by code in the standard library.
     *
     * Declaring these static, for exmaple, will cause them to be placed in the
     * .data section, which is below the heap in memory.
     */
    uint32_t idle_stack[256 / sizeof(uint32_t)];
    uint32_t scheduler_stack[1024 / sizeof(uint32_t)];
    uint32_t display_stack[2048 / sizeof(uint32_t)];
    uint32_t network_stack[(4096 + 1024) / sizeof(uint32_t)];
    uint32_t gps_stack[2048 / sizeof(uint32_t)];
    uint32_t readings_stack[4096 / sizeof(uint32_t)];

    OS_CHECK(os_initialize());

    OS_CHECK(os_task_initialize(&idle_task, "idle", OS_TASK_START_RUNNING, &task_handler_idle, NULL, idle_stack, sizeof(idle_stack)));
    OS_CHECK(os_task_initialize(&scheduler_task, "scheduler", OS_TASK_START_RUNNING, &task_handler_scheduler, NULL, scheduler_stack, sizeof(scheduler_stack)));
    if (false) {
        OS_CHECK(os_task_initialize(&display_task, "display", OS_TASK_START_RUNNING, &task_handler_display, NULL, display_stack, sizeof(display_stack)));
    }
    OS_CHECK(os_task_initialize(&network_task, "network", OS_TASK_START_RUNNING, &task_handler_network, NULL, network_stack, sizeof(network_stack)));
    OS_CHECK(os_task_initialize(&gps_task, "gps", OS_TASK_START_RUNNING, &task_handler_gps, NULL, gps_stack, sizeof(gps_stack)));
    OS_CHECK(os_task_initialize(&readings_task, "readings", OS_TASK_START_RUNNING, &task_handler_readings, NULL, readings_stack, sizeof(readings_stack)));

    auto total_stacks = sizeof(idle_stack) + sizeof(display_stack) + sizeof(network_stack) + sizeof(gps_stack) + sizeof(readings_stack) + sizeof(scheduler_stack);
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

void setup() {
    MetalModMux mmm;

    if (false) {
        FK_ASSERT(mmm.begin());
        FK_ASSERT(mmm.disable_all_modules());
    }

    board.initialize();

    board.enable_everything();

    log_configure_writer(write_log);

    log_configure_level(LogLevels::DEBUG);

    log_diagnostics();

    MetalNetwork network;
    DisplayFactory display_factory;
    Display *display = display_factory.get_display();
    SelfCheck self_check(display, &network);

    FK_ASSERT(get_buttons()->begin());

    self_check.check();

    if (false) {
        FK_ASSERT(mmm.enable_all_modules());
    }

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
