#include <loading.h>
#include <os.h>

#include "platform.h"
#include "self_check.h"
#include "httpd/server.h"
#include "board.h"
#include "hal/hal.h"
#include "hal/metal/metal.h"
#include "printf.h"

#include "secrets.h"

extern const struct fkb_header_t fkb_header;

using namespace fk;

FK_DECLARE_LOGGER("main");

static void task_handler_idle(void *params) {
    while (true) {
        fk_delay(1000);
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

        fk_delay(10);
    }
}

static void task_handler_httpd(void *params) {
    MetalNetwork network;
    #if defined(FK_WIFI_0_SSID) && defined(FK_WIFI_0_PASSWORD)
    HttpServer http_server{ &network, FK_WIFI_0_SSID, FK_WIFI_0_PASSWORD };
    #else
    HttpServer http_server{ &network };
    #endif

    auto last_changed = 0;

    while (true) {
        if (last_changed == 0 || fk_uptime() - last_changed > 5 * 60 * 1000) {
            if (http_server.enabled()) {
                http_server.stop();
            }
            else {
                if (!http_server.begin()) {
                    logerror("error starting server");
                }
            }
            last_changed = fk_uptime();
        }

        http_server.tick();

        fk_delay(10);
    }
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
    while (true) {
        fk_delay(1000);
    }
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
    os_task_t idle_task;
    uint32_t idle_stack[1024 / sizeof(uint32_t)];

    os_task_t display_task;
    uint32_t display_stack[2048 / sizeof(uint32_t)];

    os_task_t httpd_task;
    uint32_t httpd_stack[4096 / sizeof(uint32_t)];

    os_task_t gps_task;
    uint32_t gps_stack[2048 / sizeof(uint32_t)];

    os_task_t readings_task;
    uint32_t readings_stack[4096 / sizeof(uint32_t)];

    OS_CHECK(os_initialize());

    OS_CHECK(os_task_initialize(&idle_task, "idle", OS_TASK_START_RUNNING, &task_handler_idle, NULL, idle_stack, sizeof(idle_stack)));
    OS_CHECK(os_task_initialize(&display_task, "display", OS_TASK_START_RUNNING, &task_handler_display, NULL, display_stack, sizeof(display_stack)));
    OS_CHECK(os_task_initialize(&httpd_task, "httpd", OS_TASK_START_RUNNING, &task_handler_httpd, NULL, httpd_stack, sizeof(httpd_stack)));
    OS_CHECK(os_task_initialize(&gps_task, "gps", OS_TASK_START_RUNNING, &task_handler_gps, NULL, gps_stack, sizeof(gps_stack)));
    OS_CHECK(os_task_initialize(&readings_task, "readings", OS_TASK_START_RUNNING, &task_handler_readings, NULL, readings_stack, sizeof(readings_stack)));

    auto total_stacks = sizeof(idle_stack) + sizeof(display_stack) + sizeof(httpd_stack) + sizeof(gps_stack);
    loginfo("stacks = %d", total_stacks);
    loginfo("free = %lu", fk_free_memory());
    loginfo("starting os!");

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
    fkb_external_printf(f, m->uptime, level, m->facility);
    fkb_external_vprintf(fstring, args);
    fkb_external_printf(RTT_CTRL_RESET "\n");

    SEGGER_RTT_UNLOCK();

    return true;
}

void setup() {
    board.initialize();

    board.enable_everything();

    log_configure_writer(write_log);

    log_configure_level(LogLevels::DEBUG);

    loginfo("hello (memory = %lu)", fk_free_memory());

    fk_serial_number_t sn;
    fk_serial_number_get(&sn);
    loginfo("serial = %08x-%08x-%08x-%08x", sn.dwords[0], sn.dwords[1], sn.dwords[2], sn.dwords[3]);

    loginfo("fw = %s", fkb_header.firmware.name);
    char hash_string[128];
    bytes_to_hex_string(hash_string, sizeof(hash_string), fkb_header.firmware.hash, fkb_header.firmware.hash_size);
    loginfo("hash = %s", hash_string);

    MetalNetwork network;
    DisplayFactory display_factory;
    Display *display = display_factory.get_display();
    SelfCheck self_check(display, &network);

    self_check.check();

    if (fkc.slow_startup) {
        fk_delay(1000);

        display->fk_logo();

        fk_delay(1000);
    }

    run_tasks();
}

void loop() {
}
