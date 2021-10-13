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

#define OS_RWLOCK_DESIRED_NONE  0
#define OS_RWLOCK_DESIRED_READ  1
#define OS_RWLOCK_DESIRED_WRITE 2

static void blocked_enq(os_rwlock_t *rwlock, os_task_t *task) {
    blocked_append(&rwlock->blocked, task);
    OS_ASSERT(task->rwlock == NULL);
    task->rwlock = rwlock;
}

os_status_t osi_rwlock_create(os_rwlock_t *rwlock, os_rwlock_definition_t *def) {
    rwlock->def = def;
    rwlock->blocked.type = 0;
    rwlock->blocked.tasks = NULL;
    rwlock->readers = 0;
    rwlock->writers = 0;
    rwlock->flags = def->flags;
    rwlock->writer = NULL;
    return OSS_SUCCESS;
}

os_status_t osi_rwlock_acquire_read(os_rwlock_t *rwlock, uint32_t to) {
    os_task_t *task = os_task_self();

    // TODO: Allow tasks to have more than one rwlock.
    OS_ASSERT(task->rwlock == NULL || task->rwlock == rwlock);

    // Check for an easy acquire.
    if (rwlock->writers == 0) {
        rwlock->readers++;
        return OSS_SUCCESS;
    }

    if (to == 0) {
        return OSS_ERROR_TO;
    }

    // Block until somebody releases.
    task->c.desired = OS_RWLOCK_DESIRED_READ;
    blocked_enq(rwlock, task);
    svc_block(to, OS_TASK_FLAG_RWLOCK);
    return OSS_ERROR_TO;
}

os_status_t osi_rwlock_acquire_write(os_rwlock_t *rwlock, uint32_t to) {
    os_task_t *task = os_task_self();

    // TODO: Allow tasks to have more than one rwlock.
    OS_ASSERT(task->rwlock == NULL || task->rwlock == rwlock);

    // Check for an easy acquire.
    if (rwlock->readers == 0 && rwlock->writers == 0) {
        rwlock->writers++;
        rwlock->writer = task;
        return OSS_SUCCESS;
    }

    if (to == 0) {
        return OSS_ERROR_TO;
    }

    // Block until somebody releases.
    task->c.desired = OS_RWLOCK_DESIRED_WRITE;
    blocked_enq(rwlock, task);
    svc_block(to, OS_TASK_FLAG_RWLOCK);
    return OSS_ERROR_TO;
}

os_status_t osi_rwlock_release(os_rwlock_t *rwlock) {
    os_task_t *task = os_task_self();

    OS_ASSERT(rwlock->readers > 0 || rwlock->writers == 1);

    if (rwlock->readers > 0) {
        rwlock->readers--;
    }

    if (rwlock->writers > 0) {
        OS_ASSERT(rwlock->writer == task);
        rwlock->writer = NULL;
        rwlock->writers--;
    }

    /* Is somebody waiting for this rwlock? */
    if (rwlock->blocked.tasks != NULL) {
        os_task_t *task = rwlock->blocked.tasks;
        if (rwlock->readers == 0 && task->c.desired == OS_RWLOCK_DESIRED_WRITE) {
            rwlock->blocked.tasks = task->nblocked;
            task->nblocked = NULL;
            task->c.desired = OS_RWLOCK_DESIRED_NONE;
            rwlock->writers++;
            rwlock->writer = task;
            osi_task_set_stacked_return(task, OSS_SUCCESS);
            osi_dispatch_or_queue(task);
        } else {
            os_task_t *previous = NULL;
            for (os_task_t *iter = rwlock->blocked.tasks; iter != NULL; iter = iter->nblocked) {
                if (iter->c.desired == OS_RWLOCK_DESIRED_READ) {
                    if (previous == NULL) {
                        rwlock->blocked.tasks = iter->nblocked;
                    } else {
                        previous->nblocked = iter->nblocked;
                    }
                    iter->nblocked = NULL;
                    iter->c.desired = OS_RWLOCK_DESIRED_NONE;
                    osi_task_set_stacked_return(iter, OSS_SUCCESS);
                    osi_task_status_set(iter, OS_TASK_STATUS_IDLE);
                    rwlock->readers++;
                } else {
                    previous = iter;
                }
            }
        }
    }

    return OSS_SUCCESS;
}
