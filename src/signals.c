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

// TODO: A waiting mechanism? Implying a blocked queue.

os_status_t osi_signal(os_task_t *task, uint32_t signal) {
    OS_ASSERT(signal > 0);

    if (task->signal == signal) {
        return OSS_SUCCESS;
    }

    if (task->signal != 0) {
        return OSS_ERROR_INVALID;
    }

    task->signal = signal;

    return OSS_SUCCESS;
}

os_status_t osi_signal_check(uint32_t *signal) {
    os_task_t *self = os_task_self();

    *signal = self->signal;

    return OSS_SUCCESS;
}
