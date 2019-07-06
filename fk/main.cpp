#include <loading.h>
#include <os.h>

#include "platform.h"
#include "self_check.h"
#include "httpd/server.h"
#include "board.h"
#include "hal/hal.h"
#include "hal/metal/metal.h"

#include "secrets.h"

using namespace fk;

#define loginfo(f, ...)  loginfof("main", f, ##__VA_ARGS__)

#define logerror(f, ...) logerrorf("main", f, ##__VA_ARGS__)

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
                    fkerror("error starting server");
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
    uint32_t idle_stack[1024 / sizeof(uint32_t)];

    os_task_t display_task;
    uint32_t display_stack[4096 / sizeof(uint32_t)];

    os_task_t httpd_task;
    uint32_t httpd_stack[4096 / sizeof(uint32_t)];

    OS_CHECK(os_initialize());

    OS_CHECK(os_task_initialize(&idle_task, "idle", OS_TASK_START_RUNNING, &task_handler_idle, NULL, idle_stack, sizeof(idle_stack)));
    OS_CHECK(os_task_initialize(&display_task, "display", OS_TASK_START_RUNNING, &task_handler_display, NULL, display_stack, sizeof(display_stack)));
    OS_CHECK(os_task_initialize(&httpd_task, "httpd", OS_TASK_START_RUNNING, &task_handler_httpd, NULL, httpd_stack, sizeof(httpd_stack)));

    auto total_stacks = sizeof(idle_stack) + sizeof(display_stack) + sizeof(httpd_stack);
    loginfo("stacks = %d", total_stacks);
    loginfo("free = %lu", fk_free_memory());
    loginfo("starting os!");

    OS_CHECK(os_start());
}

size_t write_log(const LogMessage *m, const char *line) {
    return fkb_external_printf(RTT_CTRL_TEXT_GREEN "%08" PRIu32 RTT_CTRL_TEXT_YELLOW " %s" RTT_CTRL_RESET ": %s\n", m->uptime, m->facility, m->message);
}

void setup() {
    board.initialize();

    board.enable_everything();

    log_configure_writer(write_log);

    loginfo("hello (memory = %lu)", fk_free_memory());

    /*
    pinMode(PIN_WIRE1_SDA, OUTPUT);
    pinMode(PIN_WIRE1_SCL, OUTPUT);
    pinMode(GPS_POWER, OUTPUT);

    while (true) {
        digitalWrite(PIN_WIRE1_SDA, HIGH);
        digitalWrite(PIN_WIRE1_SCL, HIGH);
        digitalWrite(GPS_POWER, HIGH);
        delay(500);
        digitalWrite(PIN_WIRE1_SDA, LOW);
        digitalWrite(PIN_WIRE1_SCL, LOW);
        digitalWrite(GPS_POWER, LOW);
        delay(500);
    }
    */

    MetalNetwork network;
    DisplayFactory display_factory;
    Display *display = display_factory.get_display();
    SelfCheck self_check(display, &network);

    self_check.check();

    /*
    delay(1000);

    display->fk_logo();

    delay(1000);
    */

    run_tasks();
}

void loop() {
}
