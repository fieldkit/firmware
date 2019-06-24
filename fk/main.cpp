#include <loading.h>
#include <os.h>

#include "platform.h"
#include "self_check.h"

#include <Arduino.h>

using namespace fk;

static os_task_t idle_task;
static uint32_t idle_stack[OS_STACK_MINIMUM_SIZE_WORDS];

static void task_handler_idle(void *params) {
    while (true) {
        delay(1000);
        debug_println("fk: ping");
    }
}

void run_tasks() {
    OS_CHECK(os_initialize());

    OS_CHECK(os_task_initialize(&idle_task, "idle", OS_TASK_START_RUNNING, &task_handler_idle, NULL, idle_stack, sizeof(idle_stack)));

    OS_CHECK(os_start());
}

void setup() {
    debug_println("fk: hello!");

    DisplayFactory display_factory;
    Display *display = display_factory.get_display();
    SelfCheck self_check(display);

    self_check.check();

    delay(1000);

    display->fk_logo();

    delay(1000);

    while (true) {
        home_screen_t screen = {
            .time = fk_uptime(),
            .wifi = true,
            .gps = true,
            .battery = 1.0f,
        };

        display->home(screen);

        delay(10);
    }

    run_tasks();
}

void loop() {
}
