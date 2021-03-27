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
#ifndef OS_SIGNALS_H
#define OS_SIGNALS_H

#if defined(__cplusplus)
extern "C" {
#endif

os_status_t osi_signal(os_task_t *task, uint32_t signal);
os_status_t osi_signal_check(uint32_t *signal);

#if defined(__cplusplus)
}
#endif

#endif
