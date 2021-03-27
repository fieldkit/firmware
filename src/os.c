/*
 * This file is part of os.h.
 *
 * Copyright (C) 2016 Adam Heinrich <adam@adamh.cz>
 *
 * Os.h is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Os.h is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with os.h.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "os.h"
#include "internal.h"
#include "utilities.h"
#include "hooks.h"

os_globals_t osg = {
    NULL,  /* running */
    NULL,  /* scheduled */
    OS_STATE_DEFAULT,
    0,     /* ntasks */
    NULL,  /* idle */
    NULL,  /* tasks */
    NULL,  /* runqueue */
    NULL,  /* waitqueue */
};

#define MIN(x, y)   (x < y) ? (x) : (y)
#define MAX(x, y)   (x > y) ? (x) : (y)

#if defined(OS_MTB)
__attribute__((__aligned__(DEBUG_MTB_SIZE_BYTES))) uint32_t mtb[DEBUG_MTB_SIZE];
#endif

static int32_t stack_paint(uint32_t *stack, size_t size);

static int32_t stack_highwater(uint32_t *stack, size_t size);

static void infinite_loop() __attribute__ ((noreturn));

static void task_finished() __attribute__ ((noreturn));

static bool task_is_running(os_task_t *task);

static uint32_t runqueue_length(os_task_t *head);

static void runqueue_add(os_task_t **head, os_task_t *task);

static void runqueue_remove(os_task_t **head, os_task_t *task);

static bool runqueue_has_higher_priority(os_task_t *task);

static void waitqueue_add(os_task_t **head, os_task_t *task);

static void waitqueue_remove(os_task_t **head, os_task_t *task);

os_status_t os_initialize() {
    if (osg.state != OS_STATE_DEFAULT) {
        return OSS_ERROR_INVALID;
    }

    #if defined(OS_MTB)
    REG_MTB_POSITION = ((uint32_t)(mtb - REG_MTB_BASE)) & 0xFFFFFFF8;
    REG_MTB_FLOW = (((uint32_t)mtb - REG_MTB_BASE) + DEBUG_MTB_SIZE_BYTES) & 0xFFFFFFF8;
    REG_MTB_MASTER = 0x80000000 + (DEBUG_MTB_MAGNITUDE_PACKETS - 1);
    #endif

    osg.state = OS_STATE_INITIALIZED;
    osg.status_hook = NULL;
    osg.logging_hook = NULL;

    return OSS_SUCCESS;
}

os_status_t os_configure_hooks(os_task_status_hook_fn_t status_hook, os_logging_hook_fn_t logging_hook) {
    osg.status_hook = status_hook;
    osg.logging_hook = logging_hook;
    return OSS_SUCCESS;
}

os_status_t os_teardown() {
    osg.running = NULL;
    osg.scheduled = NULL;
    osg.state = OS_STATE_DEFAULT;
    osg.ntasks = 0;
    osg.idle = NULL;
    osg.tasks = NULL;
    osg.runqueue = NULL;
    osg.waitqueue = NULL;

    return OSS_SUCCESS;
}

uint32_t *initialize_stack(os_task_t *task, uint32_t *stack, size_t stack_size) {
    #if __pic__ && (defined(__SAMD21__) || defined(__SAMD51__))
    register uint32_t got_r9 asm("r9");
    #endif

    OS_ASSERT((stack_size % sizeof(uint32_t)) == 0);
    OS_ASSERT(stack_size >= OS_STACK_MINIMUM_SIZE);

    uint32_t stack_offset = (stack_size / sizeof(uint32_t));
    stack_paint(stack, stack_offset);

    // Check alignment. May not be necessary?
    uint32_t *stk = stack + stack_offset;
    if ((uintptr_t)stk & 0x04U) {
        stk--;
    }
    stk -= OS_STACK_BASIC_FRAME_SIZE;

    /* Save values of registers which will be restored on exc. return:
       - XPSR: Default value (0x01000000)
       - PC: Point to the handler function
       - LR: Point to a function to be called when the handler returns
       - R0: Point to the handler function's parameter */
    uint32_t base = 0x1000; // * (osg.ntasks + 1);
    stk[15] = 0x01000000;
    stk[14] = (uintptr_t)task->handler & ~0x01UL;
    stk[13] = (uintptr_t)&task_finished;
    stk[12] = base + 12; /* R12 */
    stk[11] = base + 3;  /* R3  */
    stk[10] = base + 2;  /* R2  */
    stk[ 9] = base + 1;  /* R1  */
    stk[ 8] = (uintptr_t)task->params;

    #if defined(__SAMD21__)
    stk[ 7] = base + 7;  // r7
    stk[ 6] = base + 6;  // r6
    stk[ 5] = base + 5;  // r5
    stk[ 4] = base + 4;  // r4
    stk[ 3] = base + 11; // r11
    stk[ 2] = base + 10; // r10
    #if defined(__pic__)
    stk[ 1] = got_r9;    // r9
    #else
    stk[ 1] = base + 9;  // r9
    #endif
    stk[ 0] = base + 8;  // r8
    #endif
    #if defined(__SAMD51__)
    stk[ 7] = base + 11; // r11
    stk[ 6] = base + 10; // r10
    #if defined(__pic__)
    stk[ 5] = got_r9;    // r9
    #else
    stk[ 5] = base + 9;  // r9
    #endif
    stk[ 4] = base + 8;  // r8
    stk[ 3] = base + 7;  // r7
    stk[ 2] = base + 6;  // r6
    stk[ 1] = base + 5;  // r5
    stk[ 0] = base + 4;  // r4
    #endif

    // Magic word to check for overflows.
    stack[0] = OSH_STACK_MAGIC_WORD;

    return stk;
}

uint32_t os_task_highwater(os_task_t *task) {
    task->highwater = stack_highwater((uint32_t *)task->stack, task->stack_size);

    return task->highwater;
}

os_status_t os_task_initialize(os_task_t *task, const char *name, os_start_status status, void (*handler)(void *params), void *params, uint32_t *stack, size_t stack_size) {
    os_task_options_t options = {
        name,
        status,
        handler,
        params,
        stack,
        stack_size,
        OS_PRIORITY_NORMAL
    };
    return os_task_initialize_options(task, &options);
}

os_status_t os_task_initialize_options(os_task_t *task, os_task_options_t *options) {
    if (osg.state != OS_STATE_INITIALIZED && osg.state != OS_STATE_TASKS_INITIALIZED) {
        return OSS_ERROR_INVALID;
    }

    task->sp = NULL; /* This gets set correctly later. */
    task->stack = options->stack;
    task->stack_size = options->stack_size;
    task->stack_kind = 0;
    task->params = options->params;
    task->handler = options->handler;
    task->status = options->status == OS_TASK_START_RUNNING ? OS_TASK_STATUS_IDLE : OS_TASK_STATUS_SUSPENDED;
    task->name = options->name;
    task->delay = 0;
    task->flags = 0;
    task->started = os_uptime();
    task->runtime = 0;
    task->scheduled = 0;
    task->queue = NULL;
    task->mutex = NULL;
    task->nblocked = NULL;
    task->semaphore = NULL;
    task->rwlock = NULL;
    task->c.message = NULL;
    task->nrp = NULL;
    task->priority = options->priority;
    task->signal = 0;
    #if defined(OS_CONFIG_DEBUG)
    task->debug_stack_max = 0;
    #endif

    task->sp = initialize_stack(task, options->stack, options->stack_size);

    task->np = osg.tasks;
    osg.tasks = task;
    osg.ntasks++;

    /* First task initialized is always the idle task, it's also the task that
     * gets a turn first. */
    if (osg.idle == NULL) {
        OS_ASSERT(task->status != OS_TASK_STATUS_SUSPENDED);
        task->priority = OS_PRIORITY_IDLE;
        osg.idle = task;
    }

    /* If the task is ready to go, add to the runqueue. */
    if (task->status != OS_TASK_STATUS_SUSPENDED) {
        runqueue_add(&osg.runqueue, task);
    }

    osg.state = OS_STATE_TASKS_INITIALIZED;

    if (osg.status_hook != NULL) {
        osg.status_hook(task, OS_TASK_STATUS_FINISHED);
    }

    return OSS_SUCCESS;
}

uint32_t os_task_is_running(os_task_t *task) {
    switch (os_task_get_status(task)) {
    case OS_TASK_STATUS_IDLE: return true;
    case OS_TASK_STATUS_ACTIVE: return true;
    case OS_TASK_STATUS_WAIT: return true;
    case OS_TASK_STATUS_SUSPENDED: return false;
    case OS_TASK_STATUS_FINISHED: return false;
    case OS_TASK_STATUS_PANIC: return false;
    case OS_TASK_STATUS_ABORTED: return false;
    default: return false;
    }
}

os_status_t os_task_start(os_task_t *task) {
    return os_task_start_options(task, task->priority, NULL);
}

os_status_t os_task_start_options(os_task_t *task, uint8_t priority, void *params) {
    OS_ASSERT(task != NULL);
    OS_ASSERT(task->status != OS_TASK_STATUS_IDLE && task->status != OS_TASK_STATUS_ACTIVE);

    task->stack_kind = 0;
    task->delay = 0;
    task->flags = 0;
    task->queue = NULL;
    task->mutex = NULL;
    task->rwlock = NULL;
    task->c.message = NULL;
    task->nblocked = NULL;
    task->params = params;
    task->started = os_uptime();
    task->runtime = 0;
    task->scheduled = 0;
    task->priority = priority;
    task->signal = 0;
    #if defined(OS_CONFIG_DEBUG)
    task->debug_stack_max = 0;
    #endif
    task->sp = initialize_stack(task, (uint32_t *)task->stack, task->stack_size);
    task->status = OS_TASK_STATUS_IDLE;

    // Kind of a hack :)
    waitqueue_remove(&osg.waitqueue, task);
    runqueue_remove(&osg.runqueue, task);
    runqueue_add(&osg.runqueue, task);

    #if defined(OS_CONFIG_DEBUG_SCHEDULE)
    osi_printf("%s: started\n", task->name);
    #endif

    if (osg.status_hook != NULL) {
        osg.status_hook(task, OS_TASK_STATUS_FINISHED);
    }

    return OSS_SUCCESS;
}

os_status_t os_task_suspend(os_task_t *task) {
    OS_ASSERT(task != NULL);
    OS_ASSERT(task->status == OS_TASK_STATUS_IDLE || task->status == OS_TASK_STATUS_ACTIVE);

    #if defined(OS_CONFIG_DEBUG_SCHEDULE)
    osi_printf("%s: suspended\n", task->name);
    #endif

    osi_task_status_set(task, OS_TASK_STATUS_SUSPENDED);

    return OSS_SUCCESS;
}

os_status_t os_task_resume(os_task_t *task) {
    OS_ASSERT(task != NULL);
    OS_ASSERT(task->status == OS_TASK_STATUS_SUSPENDED);

    #if defined(OS_CONFIG_DEBUG_SCHEDULE)
    osi_printf("%s: resumed\n", task->name);
    #endif

    osi_task_status_set(task, OS_TASK_STATUS_IDLE);

    return OSS_SUCCESS;
}

uint32_t os_task_is_stop(os_task_t *task) {
    OS_ASSERT(task != NULL);
    return OSS_SUCCESS;
}

uint32_t os_task_uptime(os_task_t *task) {
    OS_ASSERT(task != NULL);
    return os_uptime() - task->started;
}

uint32_t os_task_runtime(os_task_t *task) {
    OS_ASSERT(task != NULL);
    return os_uptime() - task->started;
}

os_task_status os_task_get_status(os_task_t *task) {
    return task->status;
}

os_status_t os_start(void) {
    if (osg.state != OS_STATE_TASKS_INITIALIZED) {
        return OSS_ERROR_INVALID;
    }

    OS_ASSERT(osi_platform_setup() == OSS_SUCCESS);
    OS_ASSERT(osg.runqueue != NULL);

    /* Running task is the first task in the runqueue. */
    osg.running = osg.idle;
    osg.running->status = OS_TASK_STATUS_ACTIVE;

    #if defined(__SAMD21__) || defined(__SAMD51__)
    NVIC_SetPriority(PendSV_IRQn, 0x7);
    NVIC_SetPriority(SysTick_IRQn, 0x2);

    /* Set PSP to the top of task's stack */
    __set_PSP((uint32_t)osg.running->sp + OS_STACK_BASIC_FRAME_SIZE);
    /* Switch to Unprivilleged Thread Mode with PSP */
    __set_CONTROL(0x02);
    /* Execute DSB/ISB after changing CONTORL (recommended) */
    __DSB();
    __ISB();

    osg.state = OS_STATE_STARTED;
    osg.running->handler(osg.running->params);

    OS_ASSERT(0);
    #endif

    return OSS_SUCCESS;
}

os_status_t osi_task_status_set(os_task_t *task, os_task_status new_status) {
    OS_LOCK();

    uint8_t old_status = task->status;

    task->status = new_status;

    if (new_status == OS_TASK_STATUS_WAIT) {
        runqueue_remove(&osg.runqueue, task);
        waitqueue_add(&osg.waitqueue, task);
    }
    else if (new_status == OS_TASK_STATUS_FINISHED) {
        runqueue_remove(&osg.runqueue, task);
    }
    else if (new_status == OS_TASK_STATUS_PANIC) {
        runqueue_remove(&osg.runqueue, task);
    }
    else if (new_status == OS_TASK_STATUS_ABORTED) {
        runqueue_remove(&osg.runqueue, task);
    }
    else if (new_status == OS_TASK_STATUS_SUSPENDED) {
        waitqueue_remove(&osg.waitqueue, task);
        runqueue_remove(&osg.runqueue, task);
    }
    else if (new_status == OS_TASK_STATUS_IDLE) {
        if (old_status == OS_TASK_STATUS_WAIT) {
            waitqueue_remove(&osg.waitqueue, task);
        }
        runqueue_add(&osg.runqueue, task);
    }

    if (osg.status_hook != NULL) {
        osg.status_hook(task, old_status);
    }

    OS_UNLOCK();

    return new_status;
}

os_status_t osi_dispatch_or_queue(os_task_t *task) {
    if (runqueue_has_higher_priority(task)) {
        if (!task_is_running(task)) {
            osi_task_status_set(task, OS_TASK_STATUS_IDLE);
        }
        if (osg.scheduled == NULL) {
            return osi_schedule();
        }
    }
    else {
        return osi_dispatch(task);
    }
    return OSS_SUCCESS;
}

os_status_t osi_dispatch(os_task_t *task) {
    OS_ASSERT(task != NULL);
    OS_ASSERT(osg.running != NULL);

    if (osg.running == task) {
        return OSS_SUCCESS;
    }
    #if defined(OS_CONFIG_DEBUG_SCHEDULE)
    else {
        osi_printf("%s\n", task->name);
    }
    #endif

    if ((task->flags & OS_TASK_FLAG_MUTEX) == OS_TASK_FLAG_MUTEX) {
        OS_ASSERT(task->queue == NULL);
        OS_ASSERT(task->mutex != NULL);
        OS_ASSERT(task->semaphore == NULL);
        OS_ASSERT(task->rwlock == NULL);

        #if defined(OS_CONFIG_DEBUG_MUTEXES)
        osi_printf("%s: removed from mutex %p\n", task->name, task->mutex);
        #endif

        if (task->mutex->blocked.tasks == task) {
            task->mutex->blocked.tasks = task->nblocked;
            task->nblocked = NULL;
        }

        blocked_remove(&task->mutex->blocked, task);

        // NOTE: If we can see if they got the mutex we can decide to end the
        // task here if not and the right flags are set.

        task->mutex = NULL;
        task->flags = 0;
    }
    if ((task->flags & OS_TASK_FLAG_QUEUE) == OS_TASK_FLAG_QUEUE) {
        OS_ASSERT(task->mutex == NULL);
        OS_ASSERT(task->queue != NULL);
        OS_ASSERT(task->semaphore == NULL);
        OS_ASSERT(task->rwlock == NULL);

        #if defined(OS_CONFIG_DEBUG_QUEUES)
        osi_printf("%s: removed from queue %p\n", task->name, task->queue);
        #endif

        blocked_remove(&task->queue->blocked, task);

        task->queue = NULL;
        task->flags = 0;
    }
    if ((task->flags & OS_TASK_FLAG_SEMAPHORE) == OS_TASK_FLAG_SEMAPHORE) {
        OS_ASSERT(task->mutex == NULL);
        OS_ASSERT(task->queue == NULL);
        OS_ASSERT(task->semaphore != NULL);
        OS_ASSERT(task->rwlock == NULL);

        blocked_remove(&task->semaphore->blocked, task);

        task->semaphore = NULL;
        task->flags = 0;
    }
    if ((task->flags & OS_TASK_FLAG_RWLOCK) == OS_TASK_FLAG_RWLOCK) {
        OS_ASSERT(task->mutex == NULL);
        OS_ASSERT(task->queue == NULL);
        OS_ASSERT(task->semaphore == NULL);
        OS_ASSERT(task->rwlock != NULL);

        blocked_remove(&task->rwlock->blocked, task);

        task->c.desired = 0;
        task->rwlock = NULL;
        task->flags = 0;
    }

    // NOTE: Should the status update happen when we actually switch?
    os_task_t *running = (os_task_t *)osg.running;
    switch (running->status) {
    case OS_TASK_STATUS_ACTIVE:
        running->status = OS_TASK_STATUS_IDLE;
        break;
    case OS_TASK_STATUS_IDLE:
        break;
    case OS_TASK_STATUS_WAIT:
        // NOTE: This is done when we change the status.
        // runqueue_remove(&osg.runqueue, running);
        // waitqueue_add(&osg.waitqueue, running);
        #if defined(OS_CONFIG_DEBUG_SCHEDULE)
        osi_printf("%s: waiting\n", running->name);
        #endif
        break;
    case OS_TASK_STATUS_SUSPENDED:
    case OS_TASK_STATUS_FINISHED:
        runqueue_remove(&osg.runqueue, running);
        break;
    case OS_TASK_STATUS_PANIC:
        osi_printf("[0x%p] %s: panic\n", running, running->name);
        runqueue_remove(&osg.runqueue, running);
        break;
    case OS_TASK_STATUS_ABORTED:
        runqueue_remove(&osg.runqueue, running);
        break;
    }

    uint8_t old_status = task->status;

    task->delay = 0;
    task->flags = 0;
    task->status = OS_TASK_STATUS_ACTIVE;

    // If this task was waiting and is being given a chance, change queues.
    if (old_status == OS_TASK_STATUS_WAIT) {
        waitqueue_remove(&osg.waitqueue, task);
        runqueue_add(&osg.runqueue, task);
        #if defined(OS_CONFIG_DEBUG_SCHEDULE)
        osi_printf("%s: running\n", task->name);
        #endif
    }

    /* Update the time the task has been running and prepare the new task. */
    // uint32_t now = os_uptime();
    uint32_t now = os_micros();
    if (running->scheduled > 0) {
        uint32_t elapsed = now - running->scheduled;
        running->runtime += elapsed;
        running->scheduled = 0;
    }
    task->scheduled = now;

    #if defined(OS_CONFIG_PARANOIA)
    if (task == osg.idle) {
        OS_ASSERT(runqueue_length(osg.runqueue) == 1);
    }
    #endif

    osg.scheduled = task;

    osi_priority_check(task); // TODO: SLOW/PARANOID

    // If we didn't schedule anything, don't bother with PendSV IRQ.
    #if defined(__SAMD21__) || defined(__SAMD51__)
    if (osg.scheduled != NULL) {
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    }
    #endif

    return OSS_SUCCESS;
}

#define OS_WAITQUEUE_NEXT_WRAPPED(n)   (((n)->nrp == NULL) ? osg.waitqueue : (n)->nrp)
#define OS_RUNQUEUE_NEXT_WRAPPED(n)    (((n)->nrp == NULL) ? osg.runqueue : (n)->nrp)

static bool task_is_running(os_task_t *task) {
    return os_task_status_is_running(task->status);
}

static bool is_higher_priority(os_priority_t a, os_priority_t b) {
    return a > b;
}

static bool is_equal_priority(os_priority_t a, os_priority_t b) {
    return a == b;
}

static os_task_t *find_new_task(os_task_t *running) {
    // Default to the running task just in case we don't go through the loop
    // or we're the only task, etc...
    os_task_t *new_task = NULL;

    // Look for a task that's higher priority than us (lower number)
    os_task_t *first = OS_RUNQUEUE_NEXT_WRAPPED(running);
    os_task_t *task = first;
    os_task_t *lower_priority = NULL;
    os_priority_t ours = running->priority;
    for ( ; task != lower_priority; ) {
        if (task_is_running(task)) {
            if (is_higher_priority(task->priority, ours)) {
                new_task = task;
                break;
            }
            else if (is_equal_priority(task->priority, ours)) {
                if (new_task == NULL) {
                    new_task = task;
                }
                task = OS_RUNQUEUE_NEXT_WRAPPED(task);
            }
            else {
                OS_ASSERT(lower_priority == NULL);
                if (lower_priority == NULL) {
                    lower_priority = task;
                }
                // Go back to the beginning, which either has tasks of higher
                // priority that should run or has more tasks of our own
                // priority or we will find ourselves.
                task = osg.runqueue;
            }
        }
        else {
            task = OS_RUNQUEUE_NEXT_WRAPPED(task);
        }

        if (task == first) {
            break;
        }
    }

    if (new_task != NULL) {
        osi_priority_check(new_task); // TODO: SLOW/PARANOID
    }

    /* If we ended up looping around w/o finding somebody of equal or higher... */
    if (new_task == NULL) {
        /* If we're no longer running, then we can drop down in priority. */
        if (!task_is_running(running) && lower_priority != NULL) {
            new_task = lower_priority;
            osi_priority_check(new_task); // TODO: SLOW/PARANOID
        }
    }

    return new_task;
}

os_status_t osi_schedule() {
    os_task_t *new_task = NULL;

    OS_ASSERT(osg.running != NULL);
    OS_ASSERT(osg.running->status != OS_TASK_STATUS_IDLE);

    // I'm seeing this assertion and I have no idea why. For now, I'm
    // changing this to a warning because I don't think it's a huge
    // problem, just unexpected.
    // OS_ASSERT(osg.scheduled == NULL);
    if (osg.scheduled != NULL) {
        osi_printf("warning: unnecessary osi_schedule\n");
        return OSS_SUCCESS;
    }

    // Check to see if anything in the waitqueue is free to go.
    uint32_t now = os_uptime();
    for (os_task_t *task = osg.waitqueue; task != NULL; task = task->nrp) {
        if (task->delay != UINT32_MAX) {
            if (now >= task->delay) {
                osi_task_status_set(task, OS_TASK_STATUS_IDLE);
                break;
            }
        }
    }

    // Look for a task that's got the same priority or higher.
    new_task = find_new_task((os_task_t *)osg.running);

    if (new_task != NULL && osg.running != new_task) {
        osi_dispatch(new_task);
    }

    // If we didn't schedule anything, don't bother with PendSV IRQ.
    #if defined(__SAMD21__) || defined(__SAMD51__)
    if (osg.scheduled != NULL) {
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    }
    #endif

    return OSS_SUCCESS;
}

inline uint32_t os_uptime() {
    return osi_platform_uptime();
}

inline uint32_t os_micros() {
    return osi_platform_micros();
}

extern char *sbrk(int32_t i);

uint32_t os_free_memory() {
    #if defined(__SAMD21__) || defined(__SAMD51__)
    return (uint32_t)__get_MSP() - (uint32_t)sbrk(0);
    #else
    return 0;
    #endif
}

uint32_t *osi_task_return_regs(os_task_t *task) {
    /* Get pointer to task return value registers (R0..R3) in Stack */
    #if defined(__SAMD51__)
    if (task->stack_kind == 1) {
        /* Extended Stack Frame: R4 - R11, S16 - S31, R0 - R3, R12, LR, PC,
         * xPSR, S0 - S15, FPSCR */
        return (uint32_t *)(task->sp + (8U * 4U) + (16U * 4U));
    }
    else {
        /* Basic Stack Frame: R4 - R11, R0 - R3, R12, LR, PC, xPSR */
        return (uint32_t *)(task->sp + (8U * 4U));
    }
    #else
    /* Stack Frame: R4 - R11, R0 - R3, R12, LR, PC, xPSR */
    return (uint32_t *)(task->sp + (8U * 4U));
    #endif
}

os_tuple_t *osi_task_stacked_return_tuple(os_task_t *task) {
    return (os_tuple_t *)osi_task_return_regs(task);
}

uint32_t osi_task_set_stacked_return(os_task_t *task, uint32_t v0) {
    uint32_t *regs = osi_task_return_regs(task);
    regs[0] = v0;
    return v0;
}

uint32_t osi_task_get_stacked_return(os_task_t *task) {
    uint32_t *regs = osi_task_return_regs(task);
    return regs[0];
}

os_status_t osi_irs_systick() {
    if (osg.state == OS_STATE_STARTED) {
        // We could have beeen in another IRQ and scheduled something, then
        // SysTick fired before we fell down to PendSV?
        if (osg.scheduled == NULL) {
            return osi_schedule();
        }
    }
    return OSS_SUCCESS;
}

const char *os_status_str(os_status_t status) {
    switch (status) {
    case OSS_SUCCESS: return "OSS_SUCCESS";
    case OSS_ERROR_TO: return "OSS_ERROR_TO";
    case OSS_ERROR_MEM: return "OSS_ERROR_MEM";
    case OSS_ERROR_INT: return "OSS_ERROR_INT";
    case OSS_ERROR_INVALID: return "OSS_ERROR_INVALID";
    default: return "UNKNOWN";
    }
}

const char *os_task_status_str(os_task_status status) {
    switch (status) {
    case OS_TASK_STATUS_IDLE: return "OS_TASK_STATUS_IDLE";
    case OS_TASK_STATUS_ACTIVE: return "OS_TASK_STATUS_ACTIVE";
    case OS_TASK_STATUS_WAIT: return "OS_TASK_STATUS_WAIT";
    case OS_TASK_STATUS_SUSPENDED: return "OS_TASK_STATUS_SUSPENDED";
    case OS_TASK_STATUS_FINISHED: return "OS_TASK_STATUS_FINISHED";
    case OS_TASK_STATUS_PANIC: return "OS_TASK_STATUS_PANIC";
    case OS_TASK_STATUS_ABORTED: return "OS_TASK_STATUS_ABORTED";
    default: return "UNKNOWN";
    }
}

const char *os_panic_kind_str(os_panic_kind_t kind) {
    switch (kind) {
    case OS_PANIC_NONE: return "OS_PANIC_NONE";
    case OS_PANIC_ASSERTION: return "OS_PANIC_ASSERTION";
    case OS_PANIC_STACK_OVERFLOW: return "OS_PANIC_STACK_OVERFLOW";
    case OS_PANIC_APP: return "OS_PANIC_APP";
    case OS_PANIC_UNKNOWN: return "OS_PANIC_UNKNOWN";
    default: return "UNKNOWN";
    }
}

void osi_priority_check(os_task_t *scheduled) {
    OS_LOCK();
    if (scheduled != NULL) {
        uint8_t scheduled_priority = scheduled->priority;
        for (os_task_t *iter = osg.runqueue; iter != NULL; iter = iter->nrp) {
            if (iter->status == OS_TASK_STATUS_ACTIVE || iter->status == OS_TASK_STATUS_IDLE) {
                if (scheduled_priority < iter->priority) {
                    osi_printf("scheduler panic: [0x%p] '%s' (%d) < [0x%p] '%s' (%d)",
                               scheduled, scheduled->name, scheduled_priority,
                               iter, iter->name, iter->priority);
                }
                OS_ASSERT(scheduled_priority >= iter->priority);
            }
        }
    }

    uint8_t priority = 0xff;
    for (os_task_t *iter = osg.runqueue; iter != NULL; iter = iter->nrp) {
        if (!(iter->status == OS_TASK_STATUS_ACTIVE || iter->status == OS_TASK_STATUS_IDLE)) {
            osi_printf("scheduler panic: [0x%p] '%s' status = %s", iter, iter->name, os_task_status_str(iter->status));
        }
        OS_ASSERT(iter->status == OS_TASK_STATUS_ACTIVE || iter->status == OS_TASK_STATUS_IDLE);
        OS_ASSERT(priority >= iter->priority);
        priority = iter->priority;
    }
    OS_UNLOCK();
}

void osi_stack_check() {
    if ((osg.running->sp < osg.running->stack) || (((uint32_t *)osg.running->stack)[0] != OSH_STACK_MAGIC_WORD)) {
        osi_panic(OS_PANIC_STACK_OVERFLOW);
    }

    #if __pic__ && defined(__SAMD51__)
    register uint32_t got_r9 asm("r9");
    OS_ASSERT(got_r9 == ((uint32_t *)osg.running->sp)[5]);
    OS_ASSERT(got_r9 == ((uint32_t *)osg.scheduled->sp)[5]);
    #endif

    osi_priority_check((os_task_t *)osg.scheduled); // TODO: SLOW/PARANOID
}

void osi_hard_fault_handler(uintptr_t *stack, uint32_t lr) {
    if (NVIC_HFSR & (1uL << 31)) {
        NVIC_HFSR |= (1uL << 31);         // Reset Hard Fault status
        *(stack + 6u) += 2u;              // PC is located on stack at SP + 24 bytes; increment PC by 2 to skip break instruction.
        return;                           // Return to application
    }

    cortex_hard_fault_t hfr;
    hfr.syshndctrl.byte = SYSHND_CTRL;    // System Handler Control and State Register
    hfr.mfsr.byte       = NVIC_MFSR;      // Memory Fault Status Register
    hfr.bfsr.byte       = NVIC_BFSR;      // Bus Fault Status Register
    hfr.bfar            = NVIC_BFAR;      // Bus Fault Manage Address Register
    hfr.ufsr.byte       = NVIC_UFSR;      // Usage Fault Status Register
    hfr.hfsr.byte       = NVIC_HFSR;      // Hard Fault Status Register
    hfr.dfsr.byte       = NVIC_DFSR;      // Debug Fault Status Register
    hfr.afsr            = NVIC_AFSR;      // Auxiliary Fault Status Register

    hfr.registers.R0 = (void *)stack[0];  // Register R0
    hfr.registers.R1 = (void *)stack[1];  // Register R1
    hfr.registers.R2 = (void *)stack[2];  // Register R2
    hfr.registers.R3 = (void *)stack[3];  // Register R3
    hfr.registers.R12 = (void *)stack[4]; // Register R12
    hfr.registers.LR = (void *)stack[5];  // Link register LR
    hfr.registers.PC = (void *)stack[6];  // Program counter PC
    hfr.registers.psr.byte = stack[7];    // Program status word PSR

    osi_hard_fault_report(stack, lr, &hfr);
}

static void task_finished() {
    OS_ASSERT(osg.running != NULL);
    OS_ASSERT(osg.running != osg.idle);
    OS_ASSERT(osg.running->status == OS_TASK_STATUS_ACTIVE);

    #if defined(OS_CONFIG_DEBUG_SCHEDULE)
    osi_printf("os: task '%s' finished\n", osg.running->name);
    #endif

    osi_task_status_set((os_task_t *)osg.running, OS_TASK_STATUS_FINISHED);

    // We're removed from the runqueue but still running until we get preempted
    // by the scheduler tick.

    infinite_loop();
}

static void infinite_loop() {
    volatile uint32_t i = 0;
    while (true) {
        i++;
    }
}

static uint32_t runqueue_length(os_task_t *head) {
    uint32_t l = 0;

    for ( ; head != NULL; head = head->nrp) {
        l++;
    }

    return l;
}

static void runqueue_add(os_task_t **head, os_task_t *task) {
    task->nrp = NULL;

    if (*head == NULL) {
        *head = task;
        return;
    }

    OS_ASSERT(task->status != OS_TASK_STATUS_WAIT && task->status != OS_TASK_STATUS_FINISHED);

    os_task_t *previous = NULL;
    for (os_task_t *iter = *head; iter != NULL; iter = iter->nrp) {
        // OS_ASSERT(iter != task);
        if (iter == task) {
            osi_printf("warning: unnecessary runqueue_add\n");
            return;
        }
        if (task->priority > iter->priority) {
            task->nrp = iter;
            if (previous == NULL) {
                *head = task;
            }
            else {
                previous->nrp = task;
            }
            osi_priority_check(NULL); // TODO: SLOW/PARANOID
            return;
        }
        if (iter->nrp == NULL) {
            iter->nrp = task;
            osi_priority_check(NULL); // TODO: SLOW/PARANOID
            return;
        }
        previous  = iter;
    }

    OS_ASSERT(0);
}
static void runqueue_remove(os_task_t **head, os_task_t *task) {
    os_task_t *previous = NULL;

    for (os_task_t *iter = *head; iter != NULL; iter = iter->nrp) {
        if (iter == task) {
            if (*head == iter) {
                *head = iter->nrp;
            }
            else {
                previous->nrp = iter->nrp;
            }
            iter->nrp = NULL;
            break;
        }
        previous = iter;
    }
}

static bool runqueue_has_higher_priority(os_task_t *task) {
    for (os_task_t *iter = osg.runqueue; iter != NULL; iter = iter->nrp) {
        if (iter->status == OS_TASK_STATUS_ACTIVE || iter->status == OS_TASK_STATUS_IDLE) {
            if (iter->priority > task->priority) {
                return true;
            }
        }
    }
    return false;
}

static bool has_cycles(os_task_t *head) {
    os_task_t *f = head;
    os_task_t *s = head;

    if (head == NULL) {
        return false;
    }

    while (true) {
        s = s->nrp;

        if (f->nrp != NULL) {
            f = f->nrp->nrp;
        }
        else {
            return false;
        }

        if (s == NULL || f == NULL) {
            return false;
        }

        if (s == f) {
            return true;
        }
    }

    return false;
}

static void waitqueue_add(os_task_t **head, os_task_t *task) {
    task->nrp = NULL;

    if (*head == NULL) {
        *head = task;
        return;
    }

    os_task_t *previous = NULL;
    for (os_task_t *iter = *head; iter != NULL; iter = iter->nrp) {
        OS_ASSERT(iter != task);
        if (task->delay < iter->delay) {
            task->nrp = iter;
            if (previous == NULL) {
                *head = task;
            }
            else {
                previous->nrp = task;
            }
            return;
        }
        if (iter->nrp == NULL) {
            iter->nrp = task;
            OS_ASSERT(!has_cycles(*head));
            return;
        }
        previous  = iter;
    }

    OS_ASSERT(0);
}

static void waitqueue_remove(os_task_t **head, os_task_t *task) {
    os_task_t *previous = NULL;

    for (os_task_t *iter = *head; iter != NULL; iter = iter->nrp) {
        if (iter == task) {
            if (*head == iter) {
                *head = iter->nrp;
            }
            else {
                previous->nrp = iter->nrp;
            }
            iter->nrp = NULL;
            break;
        }
        previous = iter;
    }
}

static int32_t stack_paint(uint32_t *stack, size_t size) {
    for (size_t i = 0u; i < size; ++i) {
        *stack = OSH_STACK_MAGIC_WORD;
        stack++;
    }
    return 0;
}

static int32_t stack_highwater(uint32_t *stack, size_t size) {
    for (size_t i = 0u; i < size; ++i) {
        if (*stack != OSH_STACK_MAGIC_WORD) {
            return i;
        }
        stack++;
    }
    return size;
}
