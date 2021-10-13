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

static void blocked_enq(os_queue_t *queue, os_task_t *task) {
    blocked_append(&queue->blocked, task);
    OS_ASSERT(task->queue == NULL);
    task->queue = queue;
}

static os_task_t *blocked_deq(os_queue_t *queue) {
    os_task_t *task = queue->blocked.tasks;
    if (task == NULL) {
        return NULL;
    }
    OS_ASSERT(task->queue == queue);
    queue->blocked.tasks = task->nblocked;
    task->nblocked = NULL;
    return task;
}

os_status_t osi_queue_create(os_queue_t *queue, os_queue_definition_t *def) {
    queue->def = def;
    queue->size = def->size;
    queue->number = 0;
    queue->first = 0;
    queue->last = 0;
    queue->status = OS_QUEUE_FINE;
    queue->blocked.type = 0;
    queue->blocked.tasks = NULL;
    for (uint16_t i = 0; i < queue->size; ++i) {
        queue->messages[i] = NULL;
    }
    return OSS_SUCCESS;
}

os_tuple_t osi_queue_enqueue_isr(os_queue_t *queue, void *message) {
    os_tuple_t tuple;
    tuple.status = OSS_SUCCESS;
    tuple.value.u32 = 0;

    /* If there's tasks waiting, we can send directly via their stack if allowed. */
    if ((queue->def->flags & OS_QUEUE_FLAGS_QUEUE_ONLY) == 0) {
        if (queue->blocked.tasks != NULL && queue->status == OS_QUEUE_BLOCKED_RECEIVE) {
            os_task_t *blocked_receiver = blocked_deq(queue);

            os_tuple_t *receive_rv = osi_task_stacked_return_tuple(blocked_receiver);
            receive_rv->status = OSS_SUCCESS;
            receive_rv->value.ptr = message;

            osi_dispatch_or_queue(blocked_receiver);

// Manually trigger this since we're inside an arbitrary ISR this won't
// necessarily happen. Our SVC handler will check for scheduled/running
// changes and do the switch w/o the PendSV.
#if defined(__SAMD21__) || defined(__SAMD51__)
            SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
#endif

            return tuple;
        }
    }

    if (queue->number == queue->size) {
        tuple.status = OSS_ERROR_MEM;
        return tuple;
    }

    // There's room, so place the message into the end of the queue.
    queue->status = OS_QUEUE_FINE;
    queue->messages[queue->first] = message;
    queue->number++;
    if (++queue->first == queue->size) {
        queue->first = 0U;
    }

    return tuple;
}

os_status_t osi_queue_enqueue(os_queue_t *queue, void *message, uint32_t to) {
    os_task_t *running = os_task_self();

    OS_ASSERT(running != NULL); // TODO: Relax this?
    OS_ASSERT(running->nblocked == NULL);

    /* If there's tasks waiting, we can send directly via their stack. */
    if ((queue->def->flags & OS_QUEUE_FLAGS_QUEUE_ONLY) == 0) {
        if (queue->blocked.tasks != NULL && queue->status == OS_QUEUE_BLOCKED_RECEIVE) {
            os_task_t *blocked_receiver = blocked_deq(queue);

            os_tuple_t *receive_rv = osi_task_stacked_return_tuple(blocked_receiver);
            receive_rv->status = OSS_SUCCESS;
            receive_rv->value.ptr = message;

            osi_dispatch_or_queue(blocked_receiver);

            return OSS_SUCCESS;
        }
    }

    if (queue->number == queue->size) {
        if (to == 0) {
            return OSS_ERROR_MEM;
        }

        // Store the message we're trying to queue, it'll be queued when space
        // becomes available.
        running->c.message = message;

        // Block until somebody takes one, freeing space.
        queue->status = OS_QUEUE_BLOCKED_SEND;
        blocked_enq(queue, os_task_self());
        svc_block(to, OS_TASK_FLAG_QUEUE);

        return OSS_ERROR_TO;
    }

    // There's room, so place the message into the end of the queue.
    queue->status = OS_QUEUE_FINE;
    queue->messages[queue->first] = message;
    queue->number++;
    if (++queue->first == queue->size) {
        queue->first = 0U;
    }

    return OSS_SUCCESS;
}

os_status_t osi_queue_dequeue(os_queue_t *queue, void **message, uint32_t to) {
    OS_ASSERT(osg.running != NULL); // TODO: Relax this?
    OS_ASSERT(osg.running->nblocked == NULL);

    if (queue->number > 0) {
        *message = queue->messages[queue->last];
        if (++queue->last == queue->size) {
            queue->last = 0;
        }
        queue->number--;

        // Is somebody waiting to send a message?
        if (queue->blocked.tasks != NULL && queue->status == OS_QUEUE_BLOCKED_SEND) {
            os_task_t *blocked_sender = blocked_deq(queue);

            queue->messages[queue->first] = blocked_sender->c.message;
            if (++queue->first == queue->size) {
                queue->first = 0U;
            }
            queue->number++;
            blocked_sender->c.message = NULL;

            os_tuple_t *send_rv = osi_task_stacked_return_tuple(blocked_sender);
            send_rv->status = OSS_SUCCESS;
            send_rv->value.ptr = NULL;

            osi_dispatch_or_queue(blocked_sender);
        }
        return OSS_SUCCESS;
    }

    if (to == 0) {
        return OSS_ERROR_MEM;
    }

    // Block for to ms or until a message comes in.
    queue->status = OS_QUEUE_BLOCKED_RECEIVE;
    blocked_enq(queue, os_task_self());
    svc_block(to, OS_TASK_FLAG_QUEUE);
    return OSS_ERROR_TO;
}
