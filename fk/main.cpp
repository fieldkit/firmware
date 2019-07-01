#include <loading.h>
#include <os.h>

#include "platform.h"
#include "self_check.h"
#include "httpd/server.h"
#include "board.h"
#include "hal/hal.h"

#include "secrets.h"

using namespace fk;

static void task_handler_idle(void *params) {
    while (true) {
        os_delay(1000);
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

        os_delay(10);
    }
}

static void task_handler_httpd(void *params) {
    MetalWifi wifi;
    #if defined(FK_WIFI_0_SSID) && defined(FK_WIFI_0_PASSWORD)
    HttpServer http_server{ &wifi, FK_WIFI_0_SSID, FK_WIFI_0_PASSWORD };
    #else
    HttpServer http_server{ &wifi };
    #endif

    auto last_changed = 0;

    while (true) {
        if (last_changed == 0 || fk_uptime() - last_changed > 60 * 1000) {
            if (http_server.enabled()) {
                http_server.stop();
            }
            else {
                if (!http_server.begin()) {
                    fkerror("wifi missing");
                }
            }
            last_changed = fk_uptime();
        }

        http_server.tick();

        os_delay(10);
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
    uint32_t idle_stack[4096 / sizeof(uint32_t)];

    os_task_t display_task;
    uint32_t display_stack[4096 / sizeof(uint32_t)];

    os_task_t httpd_task;
    uint32_t httpd_stack[4096 / sizeof(uint32_t)];

    OS_CHECK(os_initialize());

    OS_CHECK(os_task_initialize(&idle_task, "idle", OS_TASK_START_RUNNING, &task_handler_idle, NULL, idle_stack, sizeof(idle_stack)));
    OS_CHECK(os_task_initialize(&display_task, "display", OS_TASK_START_RUNNING, &task_handler_display, NULL, display_stack, sizeof(display_stack)));
    OS_CHECK(os_task_initialize(&httpd_task, "httpd", OS_TASK_START_RUNNING, &task_handler_httpd, NULL, httpd_stack, sizeof(httpd_stack)));

    fkinfo("starting os!");

    OS_CHECK(os_start());
}

size_t write_log(const LogMessage *m, const char *line) {
    return fkb_external_printf("%06" PRIu32 " %s: %s\n", m->uptime, m->facility, m->message);
}

void setup() {
    board.initialize();
    delay(100);
    board.enable_wifi();

    log_configure_writer(write_log);

    fkinfo("hello!");

    MetalWifi wifi;
    DisplayFactory display_factory;
    Display *display = display_factory.get_display();
    SelfCheck self_check(display, &wifi);

    self_check.check();

    delay(1000);

    display->fk_logo();

    delay(1000);

    run_tasks();
}

void loop() {
}
