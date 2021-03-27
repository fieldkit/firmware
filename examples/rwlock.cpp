#include <os.h>

#include <Arduino.h>
#include <malloc.h>
#include <sam.h>

#define NUMBER_OF_READERS              (8)
#define READER_HOLD_MINIMUM            (400)
#define READER_HOLD_MAXIMUM            (800)
#define READER_DELAY_MINIMUM           (1200)
#define READER_DELAY_MAXIMUM           (3000)

#define NUMBER_OF_WRITERS              (2)
#define WRITER_HOLD_MINIMUM            (100)
#define WRITER_HOLD_MAXIMUM            (300)
#define WRITER_DELAY_MINIMUM           (1000)
#define WRITER_DELAY_MAXIMUM           (1500)

static os_task_t idle_task;
static os_task_t monitor_task;
static uint32_t idle_stack[OS_STACK_MINIMUM_SIZE_WORDS];
static os_task_t reader_tasks[NUMBER_OF_READERS];
static os_task_t writer_tasks[NUMBER_OF_WRITERS];

os_rwlock_define(rwlock, 0);

static void task_handler_idle(void *params) {
    volatile uint32_t i = 0;
    while (true) {
        i++;
    }
}

static void task_handler_reader(void *params) {
    os_printf("%s started (%d)\n", os_task_name(), __get_CONTROL());

    while (true) {
        auto started = os_uptime();
        if (os_rwlock_acquire_read(os_rwlock(rwlock), 250) == OSS_SUCCESS) {
            auto wms = random(READER_HOLD_MINIMUM, READER_HOLD_MAXIMUM);
            os_printf(RTT_CTRL_TEXT_CYAN "%s: success (%dms) r=%lu w=%lu\n" RTT_CTRL_RESET, os_task_name(), wms,
                      os_rwlock(rwlock)->readers, os_rwlock(rwlock)->writers);
            os_delay(wms);
            OS_ASSERT(os_rwlock_release(os_rwlock(rwlock)) == OSS_SUCCESS);
        }
        else {
            auto elapsed = os_uptime() - started;
            os_printf("%s: fail (after %lums)\n" RTT_CTRL_RESET, os_task_name(), elapsed);
        }

        auto wms = random(READER_DELAY_MINIMUM, READER_DELAY_MAXIMUM);
        os_delay(wms);
    }
}

static void task_handler_writer(void *params) {
    os_printf("%s started (%d)\n", os_task_name(), __get_CONTROL());

    os_delay(500);

    while (true) {
        auto started = os_uptime();
        if (os_rwlock_acquire_write(os_rwlock(rwlock), 250) == OSS_SUCCESS) {
            auto wms = random(WRITER_HOLD_MINIMUM, WRITER_HOLD_MAXIMUM);
            os_printf(RTT_CTRL_TEXT_YELLOW "%s: success (%dms) r=%lu w=%lu\n" RTT_CTRL_RESET, os_task_name(), wms,
                os_rwlock(rwlock)->readers, os_rwlock(rwlock)->writers);
            os_delay(wms);
            OS_ASSERT(os_rwlock_release(os_rwlock(rwlock)) == OSS_SUCCESS);
        }
        else {
            auto elapsed = os_uptime() - started;
            os_printf("%s: fail (after %lums) r=%lu w=%lu\n" RTT_CTRL_RESET, os_task_name(), elapsed,
                      os_rwlock(rwlock)->readers, os_rwlock(rwlock)->writers);
        }

        auto wms = random(WRITER_DELAY_MINIMUM, WRITER_DELAY_MAXIMUM);
        os_delay(wms);
    }
}

static void task_handler_monitor(void *params) {
    while (true) {
        os_delay(5000);

        os_printf("\n------------\n");
        __disable_irq();
        for (auto iter = osg.tasks; iter != nullptr; iter = iter->np) {
            osi_printf("task: %s %d\n", iter->name, iter->runtime);
        }
        __enable_irq();
        os_printf("\n------------\n");
    }
}

void setup() {
    uint32_t reader_stacks[NUMBER_OF_READERS][512];
    uint32_t writer_stacks[NUMBER_OF_WRITERS][512];
    uint32_t monitor_stack[256];

    // Call this here because things go horribly if we call from within a task.
    // Something goes south with a malloc.
    random(100, 1000);

    #if defined(HSRAM_ADDR)
    os_printf("starting: %d (0x%p + %lu) (%lu used) (%d)\n", os_free_memory(), HSRAM_ADDR, HSRAM_SIZE, HSRAM_SIZE - os_free_memory(), __get_CONTROL());
    #else
    os_printf("starting: %d\n", os_free_memory());
    #endif

    #if defined(__SAMD51__)
    os_printf("starting: DHCSR = %x\n", CoreDebug->DHCSR);
    os_printf("starting: SystemCoreClock = %lu\n", SystemCoreClock);
    os_printf("starting: SysTick=%lu CYCCNT = %lu\n", SysTick->VAL, DWT->CYCCNT);
    #endif

    OS_CHECK(os_initialize());

    OS_CHECK(os_task_initialize(&idle_task, "idle", OS_TASK_START_RUNNING, &task_handler_idle, NULL, idle_stack, sizeof(idle_stack)));

    OS_CHECK(os_task_initialize(&monitor_task, "monitor", OS_TASK_START_RUNNING, &task_handler_monitor, NULL, monitor_stack, sizeof(monitor_stack)));

    for (auto i = 0; i < NUMBER_OF_READERS; ++i) {
        char temp[32];
        os_snprintf(temp, sizeof(temp), "reader-%d", i);
        OS_CHECK(os_task_initialize(&reader_tasks[i], strdup(temp), OS_TASK_START_RUNNING, &task_handler_reader, NULL, reader_stacks[i], sizeof(reader_stacks[i])));
    }

    for (auto i = 0; i < NUMBER_OF_WRITERS; ++i) {
        char temp[32];
        os_snprintf(temp, sizeof(temp), "writer-%d", i);
        OS_CHECK(os_task_initialize(&writer_tasks[i], strdup(temp), OS_TASK_START_RUNNING, &task_handler_writer, NULL, writer_stacks[i], sizeof(writer_stacks[i])));
    }

    OS_CHECK(os_rwlock_create(os_rwlock(rwlock), os_rwlock_def(rwlock)));

    OS_CHECK(os_start());
}

void loop() {
    while (1);
}
