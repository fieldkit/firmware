/**
 *
 * This file used to be a part of os.h, and has significantly evolved.
 *
 * Huge thanks to Adam Heinrich <adam@adamh.cz> for the original inspiration.
 * See the README. for more information.
 *
 * This software is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this source code. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef OS_H
#define OS_H

#include "faults.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#if defined(ARDUINO)
#include <sam.h>
#else
#endif

#include "printf.h"
#include "segger/SEGGER_RTT.h"
#include "types.h"
#include "arduino.h"
#include "linux.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Map an os_status_t to a string for display/logging.
 */
const char *os_status_str(os_status_t status);

/**
 * Map an os_task_status_t to a string for display/logging.
 */
const char *os_task_status_str(os_task_status status);

typedef enum {
    OS_PANIC_NONE,
    OS_PANIC_ASSERTION,
    OS_PANIC_STACK_OVERFLOW,
    OS_PANIC_APP,
    OS_PANIC_UNKNOWN,
} os_panic_kind_t;

/**
 * Map an os_panic_kind_t to a string for display/logging.
 */
const char *os_panic_kind_str(os_panic_kind_t kind);

/**
 * Return the currently executing task.
 */
#define os_task_self()    ((os_task_t *)osg.running)
/*
inline os_task_t *os_task_self() {
    return (os_task_t *)osg.running;
}
*/

/**
 * Return the name of the currently executing task.
 */
inline const char *os_task_name() {
    if (osg.running == NULL) {
        return NULL;
    }
    return osg.running->name;
}

/**
 *
 */
os_status_t os_initialize();

/**
 *
 */
os_status_t os_configure_hooks(os_task_status_hook_fn_t status_hook, os_logging_hook_fn_t logging_hook);

/**
 *
 */
os_status_t os_task_initialize(os_task_t *task, const char *name,
                               os_start_status status,
                               void (*handler)(void *params), void *params,
                               uint32_t *stack, size_t stack_size);

/**
 *
 */
os_status_t os_task_initialize_options(os_task_t *task, os_task_options_t *options);

/**
 *
 */
os_status_t os_start();

/**
 *
 */
os_status_t os_teardown();

/**
 *
 */
os_task_status os_task_get_status(os_task_t *task);

/**
 *
 */
os_status_t os_task_start(os_task_t *task);

/**
 *
 */
os_status_t os_task_start_options(os_task_t *task, uint8_t priority, void *params);

/**
 *
 */
os_status_t os_task_suspend(os_task_t *task);

/**
 *
 */
os_status_t os_task_resume(os_task_t *task);

/**
 *
 */
uint32_t os_task_is_stop(os_task_t *task);

/**
 *
 */
uint32_t os_task_is_running(os_task_t *task);

/**
 *
 */
uint32_t os_task_uptime(os_task_t *task);

/**
 *
 */
uint32_t os_task_runtime(os_task_t *task);

uint32_t os_task_highwater(os_task_t *task);

/**
 *
 */
uint32_t os_uptime();

/**
 *
 */
uint32_t os_micros();

/**
 *
 */
uint32_t osi_printf(const char *f, ...);

/**
 *
 */
uint32_t osi_vprintf(const char *f, va_list args);

/**
 *
 */
uint32_t os_free_memory();

/**
 *
 */
inline int32_t os_is_running() {
    return osg.state == OS_STATE_STARTED;
}

/**
 *
 */
void osi_assert(const char *assertion, const char *file, int line);

#if defined(__cplusplus)
}
#endif

#include "service.h"
#include "syscall_plumbing.h"

#endif /* OS_H */
