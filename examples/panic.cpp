#include <os.h>

#include <Arduino.h>

static os_task_t idle_task;
static os_task_t tasks[3];

static void task_handler(void *arg) {
    auto time = (uint32_t)arg;

    __disable_irq();
    os_printf("waiting %lu\n", time);
    Serial.flush();
    __enable_irq();

    os_panic(1);
}

static void task_handler_idle(void *params) {
    volatile uint32_t i = 0;
    while (true) {
        i++;
    }
}

void setup() {
    static uint32_t idle_stack[OS_STACK_MINIMUM_SIZE_WORDS];
    static uint32_t stack1[256];
    static uint32_t stack2[256];
    static uint32_t stack3[256];

    uint32_t p1 = 4000;
    uint32_t p2 = p1 / 2;
    uint32_t p3 = p1 / 4;

#if defined(HSRAM_ADDR)
    os_printf("starting: %d (0x%p + %lu) (%lu used) (%d)\n", os_free_memory(), HSRAM_ADDR, HSRAM_SIZE, HSRAM_SIZE - os_free_memory(),
              __get_CONTROL());
#else
    os_printf("starting: %d\n", os_free_memory());
#endif

    OS_CHECK(os_initialize());

    OS_CHECK(os_task_initialize(&idle_task, "idle", OS_TASK_START_RUNNING, &task_handler_idle, NULL, idle_stack, sizeof(idle_stack)));

    OS_CHECK(os_task_initialize(&tasks[0], "task1", OS_TASK_START_RUNNING, &task_handler, (void *)p1, stack1, sizeof(stack1)));
    OS_CHECK(os_task_initialize(&tasks[1], "task2", OS_TASK_START_RUNNING, &task_handler, (void *)p2, stack2, sizeof(stack2)));
    OS_CHECK(os_task_initialize(&tasks[2], "task3", OS_TASK_START_RUNNING, &task_handler, (void *)p3, stack3, sizeof(stack3)));

    OS_CHECK(os_start());
}

void loop() {
    while (1)
        ;
}
