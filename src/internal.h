/**
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
#ifndef OS_INTERNAL_H
#define OS_INTERNAL_H

#include <stdint.h>

#include "os.h"
#include "segger/SEGGER_RTT.h"
#include "mutex.h"
#include "queue.h"
#include "semaphore.h"
#include "rwlock.h"
#include "signals.h"
#include "arduino.h"
#include "syscalls.h"
#include "faults.h"
#include "utilities.h"
#include "platform.h"

#if defined(ARDUINO)
#define OS_NORETURN  __attribute__ ((noreturn))
#else
#define OS_NORETURN
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/**
 *
 */
void osi_panic(os_panic_kind_t code);

/**
 *
 */
void osi_stack_check();

/**
 *
 */
void osi_priority_check();

/**
 *
 */
os_status_t osi_dispatch(os_task_t *task);

os_status_t osi_dispatch_or_queue(os_task_t *task);

/**
 *
 */
os_status_t osi_schedule();

/**
 *
 */
os_status_t osi_task_status_set(os_task_t *task, os_task_status new_status);

/**
 *
 */
os_status_t osi_irs_systick();

/**
 *
 */
uint32_t *osi_task_return_regs(os_task_t *task);

/**
 *
 */
os_tuple_t *osi_task_stacked_return_tuple(os_task_t *task);

/**
 *
 */
uint32_t osi_task_get_stacked_return(os_task_t *task);

/**
 *
 */
uint32_t osi_task_set_stacked_return(os_task_t *task, uint32_t v0);

#if defined(__SAMD51__)

#define OS_MAX_INTERRUPT_PRIORITY         (1 << (8 - __NVIC_PRIO_BITS))

#define OS_LOCK()   {                                                                   \
                        unsigned int LockState;                                         \
                      __asm volatile ("mrs   %0, basepri  \n\t"                         \
                                      "mov   r1, %1       \n\t"                         \
                                      "msr   basepri, r1  \n\t"                         \
                                      : "=r" (LockState)                                \
                                      : "i"(OS_MAX_INTERRUPT_PRIORITY)                  \
                                      : "r1"                                            \
                                        );

#define OS_UNLOCK()   __asm volatile ("msr   basepri, %0  \n\t"                         \
                                      :                                                 \
                                      : "r" (LockState)                                 \
                                      :                                                 \
                                      );                                                \
                    }

#endif

#if !defined(OS_LOCK)
#define OS_LOCK()
#endif

#if !defined(OS_UNLOCK)
#define OS_UNLOCK()
#endif

#if defined(__cplusplus)
}
#endif

#endif // OS_INTERNAL_H
