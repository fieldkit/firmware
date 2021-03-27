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

static void blocked_enq(os_semaphore_t *semaphore, os_task_t *task) {
    blocked_append(&semaphore->blocked, task);
    OS_ASSERT(task->semaphore == NULL);
    task->semaphore = semaphore;
}

static os_task_t *blocked_deq(os_semaphore_t *semaphore) {
    os_task_t *task = semaphore->blocked.tasks;
    if (task == NULL) {
        return NULL;
    }
    OS_ASSERT(task->semaphore == semaphore);
    semaphore->blocked.tasks = task->nblocked;
    task->nblocked = NULL;
    return task;
}

os_status_t osi_semaphore_create(os_semaphore_t *semaphore, os_semaphore_definition_t *def) {
    semaphore->def = def;
    semaphore->tokens = def->tokens;
    semaphore->blocked.type = 0;
    semaphore->blocked.tasks = NULL;
    semaphore->flags = def->flags;
    return OSS_SUCCESS;
}

os_status_t osi_semaphore_acquire(os_semaphore_t *semaphore, uint32_t to) {
    os_task_t *task = os_task_self();

    // TODO: Allow tasks to have more than one semaphore.
    OS_ASSERT(task->semaphore == NULL || task->semaphore == semaphore);

    // Check for an easy acquire.
    if (semaphore->tokens > 0) {
        semaphore->tokens--;
        return OSS_SUCCESS;
    }

    if (to == 0) {
        return OSS_ERROR_TO;
    }

    // Block until somebody releases.
    blocked_enq(semaphore, task);
    svc_block(to, OS_TASK_FLAG_SEMAPHORE);
    return OSS_ERROR_TO;
}

os_status_t osi_semaphore_release(os_semaphore_t *semaphore) {
    /* Is somebody waiting for this semaphore? */
    if (semaphore->blocked.tasks != NULL) {
        os_task_t *blocked_task = blocked_deq(semaphore);
        osi_task_set_stacked_return(blocked_task, OSS_SUCCESS);
        osi_dispatch_or_queue(blocked_task);
        return OSS_SUCCESS;
    }

    semaphore->tokens++;

    return OSS_SUCCESS;
}
