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
#ifndef OS_MUTEX_H
#define OS_MUTEX_H

#if defined(__cplusplus)
extern "C" {
#endif

os_status_t osi_mutex_create(os_mutex_t *mutex, os_mutex_definition_t *def);
os_status_t osi_mutex_acquire(os_mutex_t *mutex, uint32_t to);
os_status_t osi_mutex_release(os_mutex_t *mutex);

#if defined(__cplusplus)
}
#endif

#endif
