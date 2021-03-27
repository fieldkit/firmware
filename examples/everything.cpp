#include <os.h>

#include <Arduino.h>
#include <malloc.h>
#include <sam.h>

#define NUMBER_OF_RECEIVERS            (1)
#define RECEIVER_PROCESSING_MINIMUM    (50)
#define RECEIVER_PROCESSING_MAXIMUM    (90)

#define NUMBER_OF_SENDERS              (1)
#define SENDER_DELAY_MINIMUM           (100)
#define SENDER_DELAY_MAXIMUM           (500)

static os_task_t idle_task;
static os_task_t periodic_task;
static os_task_t sender_tasks[NUMBER_OF_SENDERS];
static os_task_t receiver_tasks[NUMBER_OF_RECEIVERS];

os_queue_define(queue, 10, OS_QUEUE_FLAGS_QUEUE_ONLY);
os_mutex_define(mutex, OS_MUTEX_FLAG_NONE);

static const char *os_pstrdup(const char *f, ...) {
    char message[64];
    va_list args;
    va_start(args, f);
    os_vsnprintf(message, sizeof(message), f, args);
    va_end(args);
    auto copy = strdup(message);
    OS_ASSERT(copy != NULL);
    return copy;
}

static bool can_start_task(os_task_t *task) {
    auto status = os_task_get_status(task);
    return status == OS_TASK_STATUS_SUSPENDED || status == OS_TASK_STATUS_FINISHED;
}

static bool start_task_if_necessary(os_task_t *task) {
    if (!can_start_task(task)) {
        return false;
    }

    os_task_start(task);
    return true;
}

static void task_handler_idle(void *params) {
    while (true) {
        os_delay(1000);
        if (can_start_task(&periodic_task)) {
            start_task_if_necessary(&periodic_task);
        }
    }
}

static void task_handler_sender(void *params) {
    os_printf("%s started (%d)\n", os_task_name(), __get_CONTROL());

    uint32_t counter = 0;
    uint32_t memory_reported = os_uptime();

    while (true) {
        auto started = os_uptime();
        auto message = (char *)os_pstrdup("message<%d>", counter++);
        auto tuple = os_queue_enqueue(os_queue(queue), message, 5 * 1000);
        auto status = tuple.status;
        if (status == OSS_SUCCESS) {
            auto wms = random(SENDER_DELAY_MINIMUM, SENDER_DELAY_MAXIMUM);
            os_printf("%s: success (%dms)\n" RTT_CTRL_RESET, os_task_name(), wms);
            os_delay(wms);
        }
        else {
            auto elapsed = os_uptime() - started;
            os_printf(RTT_CTRL_TEXT_RED "%s: fail (%s) (after %lums)\n" RTT_CTRL_RESET, os_task_name(), os_status_str(status), elapsed);
            free(message);
            os_delay(100);
        }

        if (os_uptime() - memory_reported > 10000) {
            auto mi = mallinfo();
            os_printf("memory: arena=%lu uordblks=%lu\n" RTT_CTRL_RESET, mi.arena, mi.uordblks);
            memory_reported = os_uptime();
        }
    }
}

static void task_handler_receiver(void *params) {
    os_printf("%s started (%d)\n", os_task_name(), __get_CONTROL());

    os_delay(500);

    while (true) {
        auto started = os_uptime();
        auto tuple = os_queue_dequeue(os_queue(queue), 1 * 1000);
        if (tuple.status == OSS_SUCCESS) {
            auto message = (const char *)tuple.value.ptr;
            auto wms = random(RECEIVER_PROCESSING_MINIMUM, RECEIVER_PROCESSING_MAXIMUM);
            os_printf(RTT_CTRL_TEXT_CYAN "%s: success ('%s') (%dms)\n" RTT_CTRL_RESET, os_task_name(), message, wms);
            free((void *)message);
            os_delay(wms);
        }
        else {
            auto elapsed = os_uptime() - started;
            os_printf(RTT_CTRL_TEXT_RED "%s: fail (%s) (after %lums)\n" RTT_CTRL_RESET, os_task_name(), os_status_str(tuple.status), elapsed);
        }
    }
}

static void busy_work(int32_t value) {
    delay(value);
}

static void task_handler_periodic(void *params) {
    os_printf(RTT_CTRL_TEXT_GREEN "%s acquire\n" RTT_CTRL_RESET, os_task_name());
    auto status = os_mutex_acquire(os_mutex(mutex), UINT32_MAX);
    if (status == OSS_SUCCESS) {
        for (auto i = 0; i < 10; ++i) {
            busy_work(100);
            os_delay(10);
        }

        os_printf(RTT_CTRL_TEXT_GREEN "%s releasing\n" RTT_CTRL_RESET, os_task_name());
        OS_CHECK(os_mutex_release(os_mutex(mutex)));
    }
    else {
        OS_ASSERT(status == OSS_SUCCESS);
    }
}

void setup() {
    uint32_t idle_stack[OS_STACK_MINIMUM_SIZE_WORDS];
    uint32_t sender_stacks[NUMBER_OF_SENDERS][256];
    uint32_t receiver_stacks[NUMBER_OF_RECEIVERS][256];
    uint32_t periodic_stack[OS_STACK_MINIMUM_SIZE_WORDS * 4];

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

    for (auto i = 0; i < NUMBER_OF_RECEIVERS; ++i) {
        char temp[32];
        os_snprintf(temp, sizeof(temp), "receiver-%d", i);

        os_task_options_t receiver_task_options = {
            strdup(temp),
            OS_TASK_START_RUNNING,
            task_handler_receiver,
            nullptr,
            receiver_stacks[i],
            sizeof(receiver_stacks[i]),
            OS_PRIORITY_NORMAL - 4
        };

        OS_CHECK(os_task_initialize_options(&receiver_tasks[i], &receiver_task_options));
    }

    for (auto i = 0; i < NUMBER_OF_SENDERS; ++i) {
        char temp[32];
        os_snprintf(temp, sizeof(temp), "sender-%d", i);
        OS_CHECK(os_task_initialize(&sender_tasks[i], strdup(temp), OS_TASK_START_RUNNING, &task_handler_sender, NULL, sender_stacks[i], sizeof(sender_stacks[i])));
    }

    os_task_options_t periodic_task_options = {
        "periodic",
        OS_TASK_START_SUSPENDED,
        task_handler_periodic,
        nullptr,
        periodic_stack,
        sizeof(periodic_stack),
        OS_PRIORITY_NORMAL
    };
    OS_CHECK(os_task_initialize_options(&periodic_task, &periodic_task_options));

    OS_CHECK(os_queue_create(os_queue(queue), os_queue_def(queue)));
    OS_CHECK(os_mutex_create(os_mutex(mutex), os_mutex_def(mutex)));

    OS_CHECK(os_start());
}

void loop() {
    while (1);
}
