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
#include "os.h"
#include "internal.h"
#include "syscalls.h"
#include "platform.h"

uint32_t svc_delay(uint32_t ms) {
    OS_ASSERT(osg.running != NULL);
    OS_ASSERT(osg.scheduled != osg.running);

    // Don't overflow delay in this case.
    if (ms == UINT32_MAX) {
        osg.running->delay = UINT32_MAX;
    }
    else {
        osg.running->delay = os_uptime() + ms;
        OS_ASSERT(osg.running->delay != UINT32_MAX);
    }

    // NOTE: It's possible that something has already been scheduled, but we ned
    // to ensure that osg.running gets removed from the runqueue and added to
    // the waitqueue.
    osi_task_status_set((os_task_t *)osg.running, OS_TASK_STATUS_WAIT);

    if (osg.scheduled == NULL) {
        osi_schedule();
    }

    OS_ASSERT(osg.scheduled != NULL);
    OS_ASSERT(osg.scheduled != osg.running);

    return OSS_ERROR_TO;
}

uint32_t svc_panic(uint32_t code) {
    // Invoke the hook. This may hup the MCU.
    osi_panic((os_panic_kind_t)code);

    osi_task_status_set((os_task_t *)osg.running, OS_TASK_STATUS_PANIC);

    if (osg.scheduled == NULL) {
        osi_schedule();
    }

    OS_ASSERT(osg.scheduled != NULL);
    OS_ASSERT(osg.scheduled != osg.running);

    return OSS_SUCCESS;
}

uint32_t svc_abort(uint32_t code) {
    osi_task_status_set((os_task_t *)osg.running, OS_TASK_STATUS_ABORTED);

    if (osg.scheduled == NULL) {
        osi_schedule();
    }

    OS_ASSERT(osg.scheduled != NULL);
    OS_ASSERT(osg.scheduled != osg.running);

    return OSS_SUCCESS;
}

uint32_t svc_block(uint32_t ms, uint32_t flags) {
    osg.running->flags |= flags;
    return svc_delay(ms);
}

uint32_t svc_printf(const char *f, void *vargs) {
    va_list *args = (va_list *)vargs;
    return osi_vprintf(f, *args);
}

uint32_t svc_pstr(const char *str) {
    return osi_printf(str);
}

os_status_t svc_queue_create(os_queue_t *queue, os_queue_definition_t *def) {
    return osi_queue_create(queue, def);
}

os_tuple_return_type_t svc_queue_enqueue(os_queue_t *queue, void *message, uint32_t to) {
    os_tuple_t rtuple = { OSS_ERROR_TO, { 0 } };

    rtuple.status = osi_queue_enqueue(queue, message, to);

    return os_tuple_return_value(rtuple);
}

os_tuple_return_type_t svc_queue_dequeue(os_queue_t *queue, uint32_t to) {
    os_tuple_t rtuple = { OSS_ERROR_TO, { 0 } };

    rtuple.status = osi_queue_dequeue(queue, &rtuple.value.ptr, to);

    return os_tuple_return_value(rtuple);
}

os_status_t svc_mutex_create(os_mutex_t *mutex, os_mutex_definition_t *def) {
    return osi_mutex_create(mutex, def);
}

os_status_t svc_mutex_acquire(os_mutex_t *mutex, uint32_t to) {
    return osi_mutex_acquire(mutex, to);
}

os_status_t svc_mutex_release(os_mutex_t *mutex) {
    return osi_mutex_release(mutex);
}

os_status_t svc_semaphore_create(os_semaphore_t *semaphore, os_semaphore_definition_t *def) {
    return osi_semaphore_create(semaphore, def);
}

os_status_t svc_semaphore_acquire(os_semaphore_t *semaphore, uint32_t to) {
    return osi_semaphore_acquire(semaphore, to);
}

os_status_t svc_semaphore_release(os_semaphore_t *semaphore) {
    return osi_semaphore_release(semaphore);
}

os_status_t svc_rwlock_create(os_rwlock_t *rwlock, os_rwlock_definition_t *def) {
    return osi_rwlock_create(rwlock, def);
}

os_status_t svc_rwlock_acquire_read(os_rwlock_t *rwlock, uint32_t to) {
    return osi_rwlock_acquire_read(rwlock, to);
}

os_status_t svc_rwlock_acquire_write(os_rwlock_t *rwlock, uint32_t to) {
    return osi_rwlock_acquire_write(rwlock, to);
}

os_status_t svc_rwlock_release(os_rwlock_t *rwlock) {
    return osi_rwlock_release(rwlock);
}

os_status_t svc_signal(os_task_t *task, uint32_t signal) {
    return osi_signal(task, signal);
}

os_status_t svc_signal_check(uint32_t *signal) {
    return osi_signal_check(signal);
}

/**
 * Application facing service call wrappers.
 */
inline static bool osi_in_idle() {
    return os_task_self()->priority == OS_PRIORITY_IDLE;
}

uint32_t os_delay(uint32_t ms) {
    if (osi_in_task()) {
        if (osi_in_idle()) {
            osi_platform_delay(ms);
            return ms;
        }
        return __svc_delay(ms);
    }
    return osi_platform_delay(ms);
}

uint32_t os_block(uint32_t ms, uint32_t flags) {
    if (osi_in_task()) {
        return __svc_block(ms, flags);
    }
    return OSS_ERROR_INVALID;
}

uint32_t os_pstr(const char *str) {
    if (__get_IPSR() != 0U) {
        return OSS_ERROR_INVALID;
    }
    if (osi_in_task()) {
        return __svc_pstr(str);
    }
    return svc_pstr(str);
}

uint32_t os_panic(uint32_t code) {
    // If we're in a IRQ then this is pretty bad.
    if (__get_IPSR() != 0U) {
        osi_panic(code);
        return OSS_SUCCESS;
    }
    // In a task, we can call a SVC call and abandon the task.
    if (osi_in_task()) {
        __svc_panic(code);
        return OSS_SUCCESS;
    }
    // We're in the master thread, likely before starting the OS.
    osi_panic(code);
    return OSS_SUCCESS;
}

uint32_t os_abort(uint32_t code) {
    if (__get_IPSR() != 0U || !osi_in_task()) {
        return OSS_ERROR_INVALID;
    }
    return __svc_abort(code);
}

uint32_t os_printf(const char *f, ...) {
    uint32_t rval;
    va_list args;
    va_start(args, f);
    if (osi_in_task()) {
        rval = __svc_printf(f, &args);
    }
    else {
        rval = svc_printf(f, &args);
    }
    va_end(args);
    return rval;
}

os_status_t os_queue_create(os_queue_t *queue, os_queue_definition_t *def) {
    if (__get_IPSR() != 0U) {
        return OSS_ERROR_INVALID;
    }
    if (osi_in_task()) {
        return __svc_queue_create(queue, def);
    }
    return svc_queue_create(queue, def);
}

os_tuple_t os_queue_enqueue(os_queue_t *queue, void *message, uint32_t to) {
    if (__get_IPSR() != 0U) {
        OS_ASSERT(to == 0);
        return osi_queue_enqueue_isr(queue, message);
    }
    return __svc_queue_enqueue(queue, message, to);
}

os_tuple_t os_queue_dequeue(os_queue_t *queue, uint32_t to) {
    return __svc_queue_dequeue(queue, to);
}

os_status_t os_mutex_create(os_mutex_t *mutex, os_mutex_definition_t *def) {
    if (__get_IPSR() != 0U) {
        return OSS_ERROR_INVALID;
    }
    if (osi_in_task()) {
        return __svc_mutex_create(mutex, def);
    }
    return svc_mutex_create(mutex, def);
}

os_status_t os_mutex_acquire(os_mutex_t *mutex, uint32_t to) {
    os_status_t status = __svc_mutex_acquire(mutex, to);
    if (mutex->flags & OS_MUTEX_FLAG_ABORT_ON_TIMEOUT) {
        if (status == OSS_ERROR_TO) {
            __svc_abort(0);
        }
    }
    return status;
}

os_status_t os_mutex_release(os_mutex_t *mutex) {
    return __svc_mutex_release(mutex);
}

os_status_t os_semaphore_acquire(os_semaphore_t *semaphore, uint32_t to) {
    return __svc_semaphore_acquire(semaphore, to);
}

os_status_t os_semaphore_release(os_semaphore_t *semaphore) {
    return __svc_semaphore_release(semaphore);
}

os_status_t os_rwlock_create(os_rwlock_t *rwlock, os_rwlock_definition_t *def) {
    if (__get_IPSR() != 0U) {
        return OSS_ERROR_INVALID;
    }
    if (osi_in_task()) {
        return __svc_rwlock_create(rwlock, def);
    }
    return svc_rwlock_create(rwlock, def);
}

os_status_t os_rwlock_acquire_read(os_rwlock_t *rwlock, uint32_t to) {
    return __svc_rwlock_acquire_read(rwlock, to);
}

os_status_t os_rwlock_acquire_write(os_rwlock_t *rwlock, uint32_t to) {
    os_status_t rv = __svc_rwlock_acquire_write(rwlock, to);
    if (rv == OSS_SUCCESS) {
        OS_ASSERT(rwlock->readers == 0 && rwlock->writers == 1);
    }
    return rv;
}

os_status_t os_rwlock_release(os_rwlock_t *rwlock) {
    return __svc_rwlock_release(rwlock);
}

os_status_t os_signal(os_task_t *task, uint32_t signal) {
    return __svc_signal(task, signal);
}

os_status_t os_signal_check(uint32_t *signal) {
    return __svc_signal_check(signal);
}
