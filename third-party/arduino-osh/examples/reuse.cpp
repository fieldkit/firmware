#include <os.h>

#include <Arduino.h>

static os_task_t idle_task;
static uint32_t idle_stack[OS_STACK_MINIMUM_SIZE_WORDS];

static void task_handler_idle(void *params) {
    volatile uint32_t i = 0;
    while (true) {
        i++;
    }
}

static os_task_t child_task;
static uint32_t child_stack[256];

static void task_handler_child(void *params) {
    os_printf("task started\n");

    auto started = os_uptime();
    while (os_uptime() - started < 5000) {
        os_delay(100);
    }

    os_printf("task done\n");
}

static os_task_t main_task;
static uint32_t main_stack[256];

static void task_handler_main(void *params) {
    while (true) {
        switch (os_task_get_status(&child_task)) {
        case OS_TASK_STATUS_SUSPENDED: {
            OS_CHECK(os_task_resume(&child_task));
            break;
        }
        case OS_TASK_STATUS_FINISHED: {
            OS_CHECK(os_task_start(&child_task));
            break;
        }
        default: {
            break;
        }
        }

        os_delay(1000);
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 2000) {
    }

    #if defined(HSRAM_ADDR)
    os_printf("starting: %d (0x%p + %lu) (%lu used) (%d)\n", os_free_memory(), HSRAM_ADDR, HSRAM_SIZE, HSRAM_SIZE - os_free_memory(), __get_CONTROL());
    #else
    os_printf("starting: %d\n", os_free_memory());
    #endif

    OS_CHECK(os_initialize());

    OS_CHECK(os_task_initialize(&idle_task, "idle", OS_TASK_START_RUNNING, &task_handler_idle, NULL, idle_stack, sizeof(idle_stack)));

    OS_CHECK(os_task_initialize(&main_task, "main", OS_TASK_START_RUNNING, &task_handler_main, NULL, main_stack, sizeof(main_stack)));

    OS_CHECK(os_task_initialize(&child_task, "child", OS_TASK_START_SUSPENDED, &task_handler_child, NULL, child_stack, sizeof(child_stack)));

    OS_CHECK(os_start());
}

void loop() {
    while (1);
}
